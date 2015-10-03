/** Node.js Crypt(3) Library */

var crypto = require('crypto'),
    nativeCrypt3 = require('./build/Release/crypt3');

var signitures = {
  md5: '$1$',
  sha256: '$5$',
  sha512: '$6$'
};

function createSalt(type) {
  type = type || 'sha512';

  if(!signitures[type]) {
    throw new TypeError('Unknown salt type at crypt3.createSalt: ' + type);
  }

  return signitures[type] + generateSalt();
};

function generateSalt() {
  return crypto.randomBytes(10).toString('base64');
}

/** Crypt(3) password and data encryption.
 * @param {string} key user's typed password
 * @param {string} salt Optional salt, for example SHA-512 use "$6$salt$".
 * @returns {string} A generated hash in format $id$salt$encrypted
 * @see https://en.wikipedia.org/wiki/Crypt_(C)
 */
var crypt3 = module.exports = function(key, salt, type) {
  salt = salt || createSalt(type);
  return nativeCrypt3.crypt3(key, salt);
};

/** Create salt
 * @param {string} type The type of salt: md5, blowfish (only some linux distros), sha256 or sha512. Default is sha512.
 * @returns {string} Generated salt string
 */
crypt3.createSalt = createSalt;

/** Random salt fragment
 * @returns {string} Random salt string
 */
crypt3.randomSaltFragment = generateSalt;
