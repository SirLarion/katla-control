import crypto from 'crypto';

const {
  CONTROLLER_SECRET,
  AES_VARIANT,
  SALT_ROUNDS,
  SALT_LEN,
  KEY_LEN,
  KEY_DIGEST,
} = process.env;

const envDefined =
  CONTROLLER_SECRET &&
  AES_VARIANT &&
  SALT_ROUNDS &&
  SALT_LEN &&
  KEY_LEN &&
  KEY_DIGEST;

if (!envDefined) {
  throw new Error('Invalid environment variables');
}

const rounds = Number(SALT_ROUNDS);
const saltLen = Number(SALT_LEN);
const ivLen = saltLen;
const keyLen = Number(KEY_LEN);

export const decryptKatlaData = (encryptedData: string) => {
  const buffer = Buffer.from(encryptedData, 'base64');
  const salt = buffer.subarray(0, saltLen);
  const iv = buffer.subarray(0 + saltLen, saltLen + ivLen);
  const key = crypto.pbkdf2Sync(
    CONTROLLER_SECRET,
    salt,
    rounds,
    keyLen,
    KEY_DIGEST
  );
  const decipher = crypto.createDecipheriv(AES_VARIANT, key, iv);
  decipher.write(buffer.subarray(saltLen + ivLen));
  decipher.end();

  return decipher.read().toString();
};
