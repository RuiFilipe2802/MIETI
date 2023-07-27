from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives import hashes

class rsa_k:
	enc_password = b'secretdata'
	
	def __init__(self):
		pass


	def set_rsa_keys(self):
		private_server_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
		pem_s_key= private_server_key.private_bytes(encoding=serialization.Encoding.PEM,
					format=serialization.PrivateFormat.TraditionalOpenSSL,
					encryption_algorithm=serialization.BestAvailableEncryption(self.enc_password))

		private_client_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
		pem_c_key= private_client_key.private_bytes(encoding=serialization.Encoding.PEM,
					format=serialization.PrivateFormat.TraditionalOpenSSL,
					encryption_algorithm=serialization.BestAvailableEncryption(self.enc_password))

		pem_s_public_key = private_server_key.public_key().public_bytes(encoding=serialization.Encoding.PEM,
					format=serialization.PublicFormat.SubjectPublicKeyInfo
					)

		pem_c_public_key = private_client_key.public_key().public_bytes(encoding=serialization.Encoding.PEM,
					format=serialization.PublicFormat.SubjectPublicKeyInfo
					)


		with open("rsa_server.txt", "wb") as key_file_s, open("rsa_client.txt","wb") as key_file_c:
			key_file_s.write(pem_s_key)
			key_file_s.write(pem_c_public_key)
			key_file_c.write(pem_c_key)
			key_file_c.write(pem_s_public_key)
		key_file_c.close()
		key_file_s.close()



	def sign_message(self, key_filename, message):
		with open(key_filename, "rb") as key_file:
			private_key = serialization.load_pem_private_key(
				key_file.read(),
				password = self.enc_password
			)
		signature = private_key.sign(
				message,
				padding.PSS(
					mgf=padding.MGF1(hashes.SHA256()),
					salt_length=padding.PSS.MAX_LENGTH
				),
			hashes.SHA256()
		)

		return signature

	def sign_verify(self, key_filename, signature, message):
		with open(key_filename, "rb") as key_file:
			public_key = serialization.load_pem_public_key(
				key_file.read()
				)
		key_file.close()
		public_key.verify(
			signature,
			message,
			padding.PSS(
				mgf=padding.MGF1(hashes.SHA256()),
				salt_length=padding.PSS.MAX_LENGTH
			),
			hashes.SHA256()
		)

