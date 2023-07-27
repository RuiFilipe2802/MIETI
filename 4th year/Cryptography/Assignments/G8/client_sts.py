import asyncio, sys, os, base64, time
from cryptography.fernet import Fernet
import rsa 

from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import dh
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
    rsa1 = rsa.rsa_k()


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

    #concatenate (g^y,g^x) and sign
    cs_public_key = client_public_key_bytes + server_public_key_bytes 
    signature = rsa1.sign_message("rsa_client.txt", cs_public_key)
    
    # compute g^x^y
    shared_key = client_private_key.exchange(server_public_key)
    
    # cipher signature with shared key
       # derive session key
    derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'handshake data').derive(shared_key)
    cipher = Fernet(base64.urlsafe_b64encode(derived_key))
    signed_ct = cipher.encrypt(signature)
    
    # send ciphertext 
    
    writer.write(signed_ct)
    await writer.drain()
    signed_ct2 = await reader.read(max_message_length)
    signed_pt= cipher.decrypt(signed_ct2)

    # verify signature with public rsa and (g^x,g^y)
    sc_public_key = server_public_key_bytes + client_public_key_bytes
    try:
        rsa1.sign_verify("rsa_client.txt", signed_pt, sc_public_key)
    except Exception as e:
        print("Invalid signature")
        writer.close()
        print("[%d] disconnected: '%s' from '%r'" % (self.id, self.username, self.address))

    # both client and server are now authenticated 

    # derive session key
    derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'handshake data').derive(shared_key)
    # initialize fernet

    #cipher = Cipher(algorithms.AES(derived_key), modes.CBC(iv))
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