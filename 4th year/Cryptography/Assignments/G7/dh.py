import sys, os, base64, time

from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import dh
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives.serialization import Encoding, PublicFormat


def f_dh():
    # Generate some parameters. These can be reused.
    parameters = dh.generate_parameters(generator=2, key_size=1024)
    parameters_numbers = parameters.parameter_numbers()

    ##
    p = parameters_numbers.p
    g = parameters_numbers.g
    print("p: " + str(p))
    print("g: " + str(g))

    # generate alice private key
    alice_private_key = parameters.generate_private_key()
    ##
    alice_gx = alice_private_key.public_key().public_numbers().y
    alice_x = alice_private_key.private_numbers().x

    alice_public_key_bytes = alice_private_key.public_key().public_bytes(Encoding.PEM, PublicFormat.SubjectPublicKeyInfo)
    print("alice public key bytes: " + str(alice_public_key_bytes))

    print("alice gx: " + str(alice_gx))
    print("alice x: " + str(alice_x))
    alice_gx2 = pow(g,alice_x,p) # g**alice_x mod p
    print("alice gx2: " + str(alice_gx2))
    print("alice gx == alice gx2: " + str(alice_gx == alice_gx2))

    # generate bob private key
    bob_private_key = parameters.generate_private_key()
    ##
    bob_gy = bob_private_key.public_key().public_numbers().y
    bob_y = bob_private_key.private_numbers().x
    print("bob gy: " + str(bob_gy))
    print("bob y: " + str(bob_y))
    bob_gy2 = pow(g,bob_y,p)
    print("gy2: " + str(bob_gy2))
    print("bob gy == bob gy2: " + str(bob_gy == bob_gy2))

    # alice and bob shared key
    alice_shared_key = alice_private_key.exchange(bob_private_key.public_key())
    alice_shared_key2_int = pow(bob_gy,alice_x,p)
    alice_shared_key2_bytes = alice_shared_key2_int.to_bytes(
        (alice_shared_key2_int.bit_length()+7) // 8, 'big')
    print("alice_shared_key: " + str(base64.b64encode(alice_shared_key)))
    print("alice_shared_key2: " + str(base64.b64encode(alice_shared_key2_bytes)))
    print("alice shared key == alice shared key2: " + str(alice_shared_key == alice_shared_key2_bytes))

    bob_shared_key = bob_private_key.exchange(alice_private_key.public_key())
    bob_shared_key2_int = pow(alice_gx,bob_y,p)
    bob_shared_key2_bytes = bob_shared_key2_int.to_bytes(
        (bob_shared_key2_int.bit_length()+7) // 8, 'big')
    print("bob_shared_key: " + str(base64.b64encode(bob_shared_key)))
    print("bob_shared_key2: " + str(base64.b64encode(bob_shared_key2_bytes)))
    print("bob shared key == bob shared key2: " + str(bob_shared_key == bob_shared_key2_bytes))

    # Perform key derivation.
    alice_derived_key = HKDF(
        algorithm=hashes.SHA256(),
        length=32,
        salt=None,
        info=b'handshake data'
    ).derive(alice_shared_key)

    bob_derived_key = HKDF(
        algorithm=hashes.SHA256(),
        length=32,
        salt=None,
        info=b'handshake data'
    ).derive(bob_shared_key)

    print("alice_derived_key: " + str(base64.b64encode(alice_derived_key)))
    print("bob_derived_key: " + str(base64.b64encode(bob_derived_key)))
    print("bob derived key == alice derived key: " + str(alice_derived_key == bob_derived_key))


if __name__ == "__main__":
    f_dh()