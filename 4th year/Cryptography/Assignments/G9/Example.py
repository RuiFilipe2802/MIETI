#keytool -genkeypair -keystore random.p12 -storetype PKCS12 -storepass abcdef -alias KEYSTORE_ENTRY -keyalg RSA -keysize 4096 -validity 99999 -dname "CN=My SSL Certificate, OU=My Team, O=My Company, L=My City, ST=My State, C=SA" -ext san=dns:mydomain.com,dns:localhost,ip:127.0.0.1
#keytool -exportcert -keystore random.p12 -storepass abcdef -alias KEYSTORE_ENTRY -rfc -file random-public.pem
#keytool -list -keystore Server.p12

from OpenSSL import crypto
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import hashes

def load(filename, password):
    file = open(filename,'rb')
    file_bytes = file.read()
    file.close()
    p12store = crypto.load_pkcs12(file_bytes, password)
    certificate = p12store.get_certificate()
    secret_key = p12store.get_privatekey()
    return (certificate, secret_key)

def load_ca_cert(filename):
    file = open(filename,'rb')
    file_bytes = file.read()
    file.close()
    ca_cert = crypto.load_certificate(crypto.FILETYPE_ASN1, file_bytes)
    return ca_cert

def verify_cert(ca_cert, cert):
    store = crypto.X509Store()
    store.add_cert(ca_cert)
    store_ctx = crypto.X509StoreContext(store, cert)
    try:
        store_ctx.verify_certificate()
        return True
    except:
        return False

def print_cert(cert):
    cert_pem = crypto.dump_certificate(crypto.FILETYPE_PEM, cert)
    print(cert_pem)

def example():
    ca_cert = load_ca_cert("CA.cer")
    print_cert(ca_cert)
    ###
    (s_cert, s_sk) = load("Server.p12", b'1234')
    print_cert(s_cert)
    (c_cert, c_sk) = load("Client.p12", b'1234')
    print_cert(c_cert)
    ###
    r1 = verify_cert(ca_cert, s_cert)
    r2 = verify_cert(ca_cert, c_cert)
    print(r1)
    print(r2)
    ###
    (r_cert, s_sk) = load("random.p12", b'abcdef')
    r3 = verify_cert(ca_cert, r_cert)
    print(r3)

    ###
    s_sk_pem = crypto.dump_privatekey(crypto.FILETYPE_PEM,s_sk)
    s_sk_c = serialization.load_pem_private_key(s_sk_pem,password=None,backend=default_backend())
    s_sign = s_sk_c.sign(b'mensagem a assinar',
                         padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                         salt_length=padding.PSS.MAX_LENGTH),
                         hashes.SHA256())
    ###
    s_pk_c = s_sk_c.public_key()
    print(s_pk_c.public_bytes(serialization.Encoding.PEM,
                              serialization.PublicFormat.SubjectPublicKeyInfo))
    try:
        print(s_sk_c)
        print(s_pk_c)
        s_pk_c.verify(s_sign, b'mensagem a assinar',
                    padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                                salt_length=padding.PSS.MAX_LENGTH),
                    hashes.SHA256())
        print("OK")
    except:
      print("FAIL")

    try:
        s_pk_c.verify(s_sign, b'mensagem a assinar modificada',
                    padding.PSS(mgf=padding.MGF1(hashes.SHA256()),
                                salt_length=padding.PSS.MAX_LENGTH),
                    hashes.SHA256())
        print("OK")
    except:
        print("FAIL")








if __name__ == '__main__':
    example()