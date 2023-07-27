# Código baseado em https://docs.python.org/3.6/library/asyncio-stream.html#tcp-echo-client-using-streams
import asyncio
import socket
import os
from cryptography.hazmat.backends.interfaces import PEMSerializationBackend
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import dh
#from cryptography.hazmat.primitives.asymmetric.types import PUBLIC_KEY_TYPES
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.ciphers.aead import AESCCM
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.serialization import load_pem_public_key
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import padding
from OpenSSL import crypto
from OpenSSL.crypto import load_pkcs12, dump_privatekey, dump_certificate, FILETYPE_PEM, verify
from cryptography.hazmat.primitives.serialization import pkcs12
from cryptography.x509.base import Certificate

conn_port = 7777
max_msg_size = 9999


P = 99494096650139337106186933977618513974146274831566768179581759037259788798151499814653951492724365471316253651463342255785311748602922458795201382445323499931625451272600173180136123245441204133515800495917242011863558721723303661523372572477211620144038809673692512025566673746993593384600667047373692203583
G = 44157404837960328768872680677686802650999163226766694797650810379076416463147265401084491113667624054557335394761604876882446924929840681990106974314935015501571333024773172440352475358750668213444607353872754650805031912866692119819377041901642732455911509867728218394542745330014071040326856846990119719675

def verify_cert(ca_cert, cert):
        store = crypto.X509Store()
        store.add_cert(ca_cert)
        store_ctx = crypto.X509StoreContext(store, cert, chain=None)
        try:
            store_ctx.verify_certificate()
            print("!")
            return True
        except:
            print("d")
            return False

class Client:
    """ Classe que implementa a funcionalidade de um CLIENTE. """
    def __init__(self, sckt=None):
        """ Construtor da classe. """
        self.sckt = sckt
        self.msg_cnt = 0
        pn = dh.DHParameterNumbers(P, G)
        self.parameters = pn.parameters()
        self.private_key = self.parameters.generate_private_key()
        self.public_key = self.private_key.public_key()
        self.server_public_key = None
        self.shared_key = None
        self.key = None
        self.pem = None
        self.secret_key = None
        self.lenPEM = 0
            

    def process(self, msg=b""):
        """ Processa uma mensagem (`bytestring`) enviada pelo SERVIDOR.
            Retorna a mensagem a transmitir como resposta (`None` para
            finalizar ligação) """
        self.msg_cnt +=1
        
        if self.msg_cnt == 1:
            
            #opencliente.p12
            file = open("Cliente1.p12",'rb')
            file_bytes = file.read()
            file.close()
            pk1,ctf1,pcd=pkcs12.load_key_and_certificates(file_bytes, b'1234')
            p12store = crypto.load_pkcs12(file_bytes, b'1234')
            
            certificate = p12store.get_certificate()
            self.secret_key = p12store.get_privatekey().to_cryptography_key()
            

            self.b_cert=crypto.dump_certificate(crypto.FILETYPE_PEM, certificate)
            self.lenCert=len(self.b_cert)
                
            self.pem = self.public_key.public_bytes(encoding=serialization.Encoding.PEM,
                                                        format=serialization.PublicFormat.SubjectPublicKeyInfo,)
            self.lenPEM = len(self.pem)
            #else:
                #print("POuca")
            
            return self.pem 
        
        elif self.msg_cnt == 2:
            
            aux_msg = msg[:self.lenPEM]
            signature = msg[self.lenPEM:len(msg)-self.lenCert]
            certificate_sv= msg[len(msg)-self.lenCert:]

            server_certificate = crypto.load_certificate(crypto.FILETYPE_PEM, certificate_sv)
            self.pk_B=server_certificate.get_pubkey().to_cryptography_key()
            
            #openCA
            file = open("CA.cer",'rb')
            file_bytes = file.read()
            file.close()
            ca_cert = crypto.load_certificate(crypto.FILETYPE_ASN1, file_bytes)
            ca=crypto.dump_certificate(crypto.FILETYPE_PEM, ca_cert)

            verify_cert(ca_cert,server_certificate)

            if verify_cert:
            
                self.server_public_key = load_pem_public_key(aux_msg)
                
                self.shared_key = self.private_key.exchange(self.server_public_key)
                
                info = b'shared_key'            
                hkdf = HKDF(
                        algorithm=hashes.SHA256(),
                        length=32,
                        salt = None,
                        info = info,
                    )
                self.key = hkdf.derive(self.shared_key)
                
                message = self.pem + aux_msg
                
                self.pk_B.verify(
                    signature,
                    message,
                    padding.PSS(
                        mgf=padding.MGF1(hashes.SHA256()),
                        salt_length=padding.PSS.MAX_LENGTH
                    ),
                    hashes.SHA256()
                )
                
                sig = self.secret_key.sign(
                    message,
                    padding.PSS(
                        mgf=padding.MGF1(hashes.SHA256()),
                        salt_length=padding.PSS.MAX_LENGTH
                        ),
                hashes.SHA256()
                )
                
                print("Recebi a chave publica")
                print('Input message to send (empty to finish)')
                new_msg = input().encode()
                aesccm = AESCCM(self.key)
                nonce = os.urandom(13)
                ct = aesccm.encrypt(nonce, new_msg, None)

                print(str(len(ct))+" "+ str(len(nonce))+" "+str(len(sig))+" "+str(len(self.b_cert)))

                return  ct + nonce + sig + self.b_cert if len(ct) > 0 else None
            else:
                return 

        else:
            aesccm = AESCCM(self.key)
            nonce = msg[len(msg)-13:]
            msg = msg[:len(msg)-13]
                
            new_msg = aesccm.decrypt(nonce, msg, None)
            print('Received (%d): %r' % (self.msg_cnt-2 , new_msg.decode()))
            print('Input message to send (empty to finish)') 
            new_msg = input().encode()
            aesccm = AESCCM(self.key)
            nonce = os.urandom(13)
            ct = aesccm.encrypt(nonce, new_msg, None)
            #
            return ct + nonce if len(ct) > 0 else None 

            
        

#
#
# Funcionalidade Cliente/Servidor
#
# obs: não deverá ser necessário alterar o que se segue
#


async def tcp_echo_client():
    reader, writer = await asyncio.open_connection('127.0.0.1', conn_port)
    addr = writer.get_extra_info('peername')
    client = Client(addr)
    msg = client.process()
    while msg:
        writer.write(msg)
        msg = await reader.read(max_msg_size)
        if msg :
            msg = client.process(msg)
        else:
            break
    writer.write(b'\n')
    print('Socket closed!')
    writer.close()

def run_client():
    loop = asyncio.get_event_loop()
    loop.run_until_complete(tcp_echo_client())


run_client()