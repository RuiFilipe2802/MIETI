# Código baseado em https://docs.python.org/3.6/library/asyncio-stream.html#tcp-echo-client-using-streams
import asyncio
import os
from cryptography.hazmat.primitives.ciphers.aead import AESCCM
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives.serialization import load_pem_public_key
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import padding
from OpenSSL import crypto


conn_cnt = 0
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
        return True
    except:
        return False

class ServerWorker(object):
    """ Classe que implementa a funcionalidade do SERVIDOR. """
    def __init__(self, cnt, addr=None):
        """ Construtor da classe. """
        self.id = cnt
        self.addr = addr
        self.msg_cnt = 0
        pn = dh.DHParameterNumbers(P, G)
        self.parameters = pn.parameters()
        self.private_key = self.parameters.generate_private_key()
        self.public_key = self.private_key.public_key()
        self.public_peer_key = None
        self.key = None
        self.sig_key = None
        self.secret_key = None
        self.lensig = 0


    def process(self, msg):
        """ Processa uma mensagem (`bytestring`) enviada pelo CLIENTE.
            Retorna a mensagem a transmitir como resposta (`None` para
            finalizar ligação) """
        self.msg_cnt += 1
        #
        # ALTERAR AQUI COMPORTAMENTO DO SERVIDOR
        
        if self.msg_cnt == 1:
            #openCA
            file = open("CA.cer",'rb')
            file_bytes = file.read()
            file.close()
            self.ca_cert = crypto.load_certificate(crypto.FILETYPE_ASN1, file_bytes)
            ca=crypto.dump_certificate(crypto.FILETYPE_PEM, self.ca_cert)

            #openServidor.p12
            file1 = open("Servidor.p12",'rb')
            file_bytes1 = file1.read()
            file1.close()
            p12store = crypto.load_pkcs12(file_bytes1, b'1234')
            certificate = p12store.get_certificate()
            sec_key = p12store.get_privatekey()
            self.secret_key=sec_key.to_cryptography_key()
            b_certificate=crypto.dump_certificate(crypto.FILETYPE_PEM, certificate)

            self.lenCert=len(b_certificate)
            

            self.public_peer_key = load_pem_public_key(msg)
            self.shared_key = self.private_key.exchange(self.public_peer_key)

            pem = self.public_key.public_bytes(encoding=serialization.Encoding.PEM,
                                                        format=serialization.PublicFormat.SubjectPublicKeyInfo,)
            
            self.message = msg + pem 
            
            signature = self.secret_key.sign(
                 self.message,
                 padding.PSS(
                    mgf=padding.MGF1(hashes.SHA256()),
                    salt_length=padding.PSS.MAX_LENGTH
                    ),
             hashes.SHA256()
            )
            self.lensig = len(signature)        

            info = b'shared_key'            
            hkdf = HKDF(
                    algorithm=hashes.SHA256(),
                    length=32,
                    salt = None,
                    info = info,
                )
            self.key = hkdf.derive(self.shared_key)
            
            print("Recebi a chave publica")
            
            return pem + signature + b_certificate
        
        
        if self.msg_cnt == 2:
            nonce = msg[len(msg)-13-self.lensig-self.lenCert: len(msg)-self.lensig-self.lenCert]
            signature = msg[len(msg)-self.lensig-self.lenCert:len(msg)-self.lenCert]
            msg_a = msg[:len(msg)-13-self.lensig-self.lenCert]
            ct_certificate=msg[len(msg_a)+len(signature)+len(nonce):]
            

            client_certificate = crypto.load_certificate(crypto.FILETYPE_PEM, ct_certificate)
            self.pk_A=client_certificate.get_pubkey().to_cryptography_key()

            verify_cert(self.ca_cert,client_certificate)

            if verify_cert:
            
                self.pk_A.verify(
                    signature,
                    self.message,
                    padding.PSS(
                        mgf=padding.MGF1(hashes.SHA256()),
                        salt_length=padding.PSS.MAX_LENGTH
                    ),
                    hashes.SHA256()
                )
        else:
            nonce = msg[len(msg)-13:]
            msg_a = msg[:len(msg)-13]

        aesccm = AESCCM(self.key)
        new_msg = aesccm.decrypt(nonce, msg_a, None)
        txt = new_msg.decode()
        if not txt:
            return txt
        print('%d : %r' % (self.id,txt))
        new_msg = txt.upper().encode()
        aesccm = AESCCM(self.key)
        nonce = os.urandom(13)
        ct = aesccm.encrypt(nonce, new_msg, None)
        
        return ct + nonce if len(ct) > 0 else None


#
#
# Funcionalidade Cliente/Servidor
#
# obs: não deverá ser necessário alterar o que se segue
#


async def handle_echo(reader, writer):
    global conn_cnt
    conn_cnt +=1
    addr = writer.get_extra_info('peername')
    srvwrk = ServerWorker(conn_cnt, addr)
    data = await reader.read(max_msg_size)
    while True:
        if not data: continue
        if data[:1]==b'\n': break
        data = srvwrk.process(data)
        if not data: break
        writer.write(data)
        await writer.drain()
        data = await reader.read(max_msg_size)
    print("[%d]" % srvwrk.id)
    writer.close()


def run_server():
    loop = asyncio.get_event_loop()
    coro = asyncio.start_server(handle_echo, '127.0.0.1', conn_port, loop=loop)
    server = loop.run_until_complete(coro)
    # Serve requests until Ctrl+C is pressed
    print('Serving on {}'.format(server.sockets[0].getsockname()))
    print('  (type ^C to finish)\n')
    try:
        loop.run_forever()
    except KeyboardInterrupt:
        pass
    # Close the server
    server.close()
    loop.run_until_complete(server.wait_closed())
    loop.close()
    print('\nFINISHED!')

run_server()