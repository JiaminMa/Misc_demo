#!/usr/bin/python3
import os
import sys
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives.asymmetric import ec, rsa
from cryptography.hazmat.primitives import hashes
import codecs
from cryptography.hazmat.primitives.serialization import load_pem_private_key
from cryptography.hazmat.primitives.asymmetric.utils import decode_dss_signature, encode_dss_signature
from cryptography.x509 import load_pem_x509_certificate

from cryptography.exceptions import InvalidSignature
import binascii
from cryptography.hazmat.primitives.asymmetric import padding

def bit_to_bytes(a):
    return (a + 7) // 8

def gen_ecdsa_key(pri_key, cert_file, ecdsa_type = "SECP384"):

    #Only support ECDSA384 so far, using openssl
    gen_ecdsa_pri_key_cmd = "openssl ecparam -genkey -text -out {} -name secp384r1".format(pri_key)
    print(gen_ecdsa_pri_key_cmd)
    os.system(gen_ecdsa_pri_key_cmd)

    gen_ecdsa_cert_key_cmd = "openssl req -new -verbose -pubkey -inform PEM -key {} -x509 -nodes -days 10"  \
                            " -out {}".format(pri_key, cert_file)
    print(gen_ecdsa_cert_key_cmd)
    os.system(gen_ecdsa_cert_key_cmd)


#Only support ECDSA384
def ecdsa_sign(data_file_name, pri_key_file_name, sig_file_name):

    #Read the raw data
    data_file = open(data_file_name, "rb")
    data = data_file.read()
    data_file.close()

    #Read the pri_key_file
    pri_key_file = open(pri_key_file_name, "rb")
    key_data = pri_key_file.read()
    pri_key_file.close()

    digest = hashes.Hash(
            hashes.SHA384(),
            default_backend()
        )

    digest.update(data)
    dgst = digest.finalize()
    print("Data digest to sign: {:s}".format(dgst.hex()))


    skey = load_pem_private_key(
                key_data, password=None, backend=default_backend())

    sig_data = skey.sign(
                data,
                ec.ECDSA(hashes.SHA384())
            )
    print(str(sig_data))
    sig_r, sig_s = decode_dss_signature(sig_data)

    sig_bytes = b''
    key_size_in_bytes = bit_to_bytes(skey.public_key().key_size)
    sig_r_bytes = sig_r.to_bytes(key_size_in_bytes, "big")
    sig_bytes += sig_r_bytes
    print("ECDSA signature R: {:s}".format(sig_r_bytes.hex()))
    sig_s_bytes = sig_s.to_bytes(key_size_in_bytes, "big")
    sig_bytes += sig_s_bytes
    print("ECDSA signature S: {:s}".format(sig_s_bytes.hex()))

    print("ECDSA signautre: {:s}".format(sig_bytes.hex()))
    #Write sig to sig_file
    sig_file = open(sig_file_name, "wb")
    sig_file.write(sig_bytes)
    sig_file.close()

#Only support ECDSA384
def ecdsa_verify(data_file, sig_data, pub_key_file):


    data_f = open(data_file, "rb")
    pay_load = data_f.read()
    data_f.close()

    sig_f = open(sig_data, "rb")
    r_s = sig_f.read()
    sig_f.close()


    with open(pub_key_file, 'rb') as fpkey:
        pem_data = fpkey.read()
    cert = load_pem_x509_certificate(pem_data, default_backend())
    public_key = cert.public_key()

    if isinstance(public_key, ec.EllipticCurvePublicKey):

        sig_r = int.from_bytes(r_s[:int(len(r_s) / 2)], byteorder='big')
        sig_s = int.from_bytes(r_s[-int(len(r_s) / 2):], byteorder='big')

        signature = encode_dss_signature(sig_r, sig_s)
        try:
            public_key.verify(
                signature,
                pay_load,
                ec.ECDSA(hashes.SHA384())
            )
            print("ECDSA Correct signature detected.. :)\n")
        except InvalidSignature:
            print("ECDSA Invalid signature detected.. :(\n")
    else:
        print("RSA not yet supported")
        exit()

if __name__ == '__main__':

    if len(sys.argv) <= 1:
        print("paramter not correct")
        exit()

    if sys.argv[1] == "gen":

        if len(sys.argv) <= 3:
            print("my_ecdsa gen pri_key_file pub_key_file")
            exit()

        pri_key = sys.argv[2]
        pub_key = sys.argv[3]
        gen_ecdsa_key(pri_key, pub_key)

    if sys.argv[1] == "sign":
        if len(sys.argv) <= 3:
            print("my_ecdsa sign data pri_key_file sig_file")
            print("or my_rsa sign data pri_key_file")
            print("or my_rsa sign data pri_key_file sig_file hash")
            exit() 

        data_file = sys.argv[2]
        pri_key_file = sys.argv[3]
        if len(sys.argv)== 5:
            sig_file = sys.argv[4]
            ecdsa_sign(data_file, pri_key_file, sig_file)
        else:    
            ecdsa_sign(data_file, pri_key_file)

    if sys.argv[1] == "verify":

        if len(sys.argv) <= 4:
            print("my_rsa verify data sig_file cert_file")
            exit() 

        data_file = sys.argv[2]
        sig_data = sys.argv[3]
        pub_key_file = sys.argv[4]
        ecdsa_verify(data_file, sig_data, pub_key_file)




