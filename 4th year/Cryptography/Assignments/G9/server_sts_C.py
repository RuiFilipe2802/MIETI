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

id = 0
max_message_length = 1024 * 32

class Server(object):

    def __init__(self, id, address):
        self.id = id
        self.address = address

    async def process(self, reader, writer):
        try:

            # read username
            username = await  reader.read(max_message_length)
            self.username = username.decode()
            print("[%d] '%s' from '%r'" % (self.id,self.username, self.address), flush=True)


            # trusted certificate
            file = open("CA.cer", 'rb')
            file_bytes = file.read()
            file.close()
            ca_cert = crypto.load_certificate(crypto.FILETYPE_ASN1, file_bytes)

            # retrieving data from the .p12 file (certificate and private key)
            file = open("Server.p12",'rb')
            file_bytes = file.read()
            file.close()
            p12store = crypto.load_pkcs12(file_bytes, b'1234')
            certificate = p12store.get_certificate()
            s_secret_key = p12store.get_privatekey()

        
            # send g^x
            parameters = (dh.DHParameterNumbers(P, G)).parameters()
            server_private_key = parameters.generate_private_key();
            server_public_key_bytes = server_private_key.public_key().public_bytes(Encoding.PEM, PublicFormat.SubjectPublicKeyInfo)
            writer.write(server_public_key_bytes)
            await writer.drain()

            # receive g^y
            client_public_key_bytes = await reader.read(max_message_length)
            client_public_key = load_pem_public_key(client_public_key_bytes)

            # receive and verify certificate
            store = crypto.X509Store()
            store.add_cert(ca_cert)
            c_certificate = await reader.read(max_message_length)
            
            c_cer = crypto.load_certificate(crypto.FILETYPE_PEM, c_certificate)
            store_ctx = crypto.X509StoreContext(store, c_cer)    
            store_ctx.verify_certificate() 
              

            # compute g^b^a
            shared_key = server_private_key.exchange(client_public_key)
            cs_public_key = client_public_key_bytes + server_public_key_bytes 
            
            # derive session key
            derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'handshake data').derive(shared_key)
            cipher = Fernet(base64.urlsafe_b64encode(derived_key))
            
            # receive signed ct
            # signed_ct = await reader.read(max_message_length)
            signed_ct = await reader.read(max_message_length) 
            signed_pt = cipher.decrypt(signed_ct)
             

            # verify signature with certificate key
            c_pb_key = c_cer.get_pubkey()
            c_pbk_pem = crypto.dump_publickey(crypto.FILETYPE_PEM, c_pb_key)
            c_pbk = serialization.load_pem_public_key(c_pbk_pem, backend=None)
            c_pbk.verify(signed_pt, cs_public_key,
                    padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                                salt_length=padding.PSS.MAX_LENGTH),
                                hashes.SHA256())
            
            # sends certificate so client can verify authenticity
            s_cer_pem = crypto.dump_certificate(crypto.FILETYPE_PEM, certificate)
            writer.write(s_cer_pem)
            await writer.drain()

            # sends back ct signed with certificate
            s_sk_pem = crypto.dump_privatekey(crypto.FILETYPE_PEM, s_secret_key)
            s_sk_c = serialization.load_pem_private_key(s_sk_pem,password=None,backend=default_backend())
            s_sign = s_sk_c.sign(cs_public_key,
                                 padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                                 salt_length=padding.PSS.MAX_LENGTH),
                                 hashes.SHA256())
            
            
            # sends ct so client verifies certificate too
            signed_ct2 = cipher.encrypt(s_sign)
            writer.write(signed_ct2)
            await writer.drain()

            # Both server and client are now authenticated#

            derived_key = HKDF(algorithm=hashes.SHA256(), length=32, salt=None, info=b'handshake data').derive(shared_key)
            print("[%d] '%s' derived_key: %s " % (self.id, self.username, str(base64.b64encode(derived_key))))
            
    
            cipher = Fernet(base64.urlsafe_b64encode(derived_key))
            while True:
                message_encrypted = await reader.read(max_message_length)
                message_decrypted = cipher.decrypt(message_encrypted)
                print("[%d] '%s' : %s" % (self.id, self.username, message_encrypted))
                print("[%d] '%s' : %s" % (self.id, self.username, message_decrypted.decode('utf-8')))
        except Exception as e:
            print(e)
            writer.close()
            print("[%d] disconnected: '%s' from '%r'" % (self.id, self.username, self.address))

#@asyncio.coroutine
async def handle_echo(reader, writer):
    global id
    id += 1
    address = writer.get_extra_info('peername')
    server = Server(id, address)
    await server.process(reader,writer)

if __name__ == "__main__":
    ip = '127.0.0.1'
    port = 8888;
    if len(sys.argv) == 3:
        ip = sys.argv[1]
        port = int(sys.argv[2])

    loop = asyncio.get_event_loop()
    coro = asyncio.start_server(handle_echo, ip, port, loop=loop)
    server = loop.run_until_complete(coro)

    # Serve requests until Ctrl+C is pressed
    print('Serving on {}'.format(server.sockets[0].getsockname()))
    try:
        loop.run_forever()
    except KeyboardInterrupt:
        pass

    # Close the server
    server.close()
    loop.run_until_complete(server.wait_closed())
    loop.close()