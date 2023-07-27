import asyncio, sys, os, base64, time
from cryptography.fernet import Fernet

from OpenSSL import crypto
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import dh, padding, rsa
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat, load_pem_public_key

# from https://tools.ietf.org/html/rfc3526
#   3.  2048-bit MODP Group
P = 0xFFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF
G = 2

max_message_length = 1024 * 32

#@asyncio.coroutine
async def client(ip, port, username, loop):
    reader, writer = await asyncio.open_connection(ip, port, loop=loop)

    # Certificate that we trust
    file = open("CA.cer", "rb")
    file_bytes = file.read()
    file.close()
    ca_cert = crypto.load_certificate(crypto.FILETYPE_ASN1, file_bytes)
    
    # retrieving data from the .p12 file (certificate and private key)
    file = open("Client.p12", "rb")
    file_bytes = file.read()
    file.close()
    p12store = crypto.load_pkcs12(file_bytes, b'1234')
    certificate = p12store.get_certificate()
    c_secret_key = p12store.get_privatekey()

    # send username
    writer.write(username.encode())
    await writer.drain()

    # read g^x
    server_public_key_bytes = await reader.read(max_message_length)
    server_public_key = load_pem_public_key(server_public_key_bytes)


    # calculate g^y and send
    parameters = (dh.DHParameterNumbers(P, G)).parameters()
    client_private_key = parameters.generate_private_key()
    client_public_key_bytes = client_private_key.public_key().public_bytes(Encoding.PEM, PublicFormat.SubjectPublicKeyInfo)
    writer.write(client_public_key_bytes)
    await writer.drain()

    
    #concatenate (g^y,g^x) and sign with certificate
    cs_public_key = client_public_key_bytes + server_public_key_bytes 
    c_sk_pem = crypto.dump_privatekey(crypto.FILETYPE_PEM, c_secret_key)
    c_sk_c = serialization.load_pem_private_key(c_sk_pem,password=None,backend=default_backend())
    c_sign = c_sk_c.sign(cs_public_key,
                         padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                         salt_length=padding.PSS.MAX_LENGTH),
                         hashes.SHA256())
    # send certificate
    c_cer_pem = crypto.dump_certificate(crypto.FILETYPE_PEM, certificate)
    writer.write(c_cer_pem)
    await writer.drain()

    # compute g^x^y
    shared_key = client_private_key.exchange(server_public_key)
    
    # cipher signature with certificate key
    derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'handshake data').derive(shared_key)
    cipher = Fernet(base64.urlsafe_b64encode(derived_key))
    signed_ct = cipher.encrypt(c_sign)
    
    # send signed ciphertext 
    writer.write(signed_ct)
    await writer.drain()
    

    # verify certificate
    store = crypto.X509Store()
    store.add_cert(ca_cert)
    s_certificate = await reader.read(max_message_length)
    s_cer = crypto.load_certificate(crypto.FILETYPE_PEM, s_certificate)
    store_ctx = crypto.X509StoreContext(store, s_cer)    
    store_ctx.verify_certificate()
    # server verified as an CA    

    #receive last signed ct 
    signed_ct2 = await reader.read(max_message_length)
    signed_pt= cipher.decrypt(signed_ct2)



    # verify signature with certificate key
    s_pb_key = s_cer.get_pubkey()
    s_pbk_pem = crypto.dump_publickey(crypto.FILETYPE_PEM, s_pb_key)
    s_pbk = serialization.load_pem_public_key(s_pbk_pem, backend=None)
    s_pbk.verify(signed_pt, cs_public_key,
                    padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                                salt_length=padding.PSS.MAX_LENGTH),
                                hashes.SHA256())

    # both client and server are now authenticated #

    # derive session key
    derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'handshake data').derive(shared_key)
    # initialize fernet

    cipher = Fernet(base64.urlsafe_b64encode(derived_key))
    message = input(">")
    while message != ":quit":
        message_encrypted = cipher.encrypt(message.encode('utf-8'))
        writer.write(message_encrypted)
        await writer.drain()
        message = input(">")

    writer.close()

if __name__ == "__main__":
    ip = '127.0.0.1'
    port = 8888;
    username = "default"
    if len(sys.argv) == 4:
        ip = sys.argv[1]
        port = int(sys.argv[2])
        username = sys.argv[3]

    loop = asyncio.get_event_loop()
    loop.run_until_complete(client(ip, port, username, loop))
    loop.close()