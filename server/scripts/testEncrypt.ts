import crypto from 'crypto';
import 'dotenv/config';

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

const salt = crypto.randomBytes(saltLen);
const iv = crypto.randomBytes(ivLen);
const key = crypto.pbkdf2Sync(
  CONTROLLER_SECRET,
  salt,
  rounds,
  keyLen,
  KEY_DIGEST
);

const cipher = crypto.createCipheriv(AES_VARIANT, key, iv);

cipher.write('{ "temp_in": 20.4, "temp_out": 15.4, "humidity": 40.3 }');
cipher.end();

const encrypted = cipher.read();
console.log({
  iv: iv.toString('base64'),
  salt: salt.toString('base64'),
  encrypted: Buffer.concat([salt, iv, encrypted]).toString('base64'),
});
