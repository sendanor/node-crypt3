/** Node.js Crypt(3) Library */

var createSalt = require('./createsalt.js');

/** Crypt(3) password and data encryption.
 * @param {string} key user's typed password
 * @param {string} salt Optional salt, for example SHA-512 use "$6$salt$".
 * @param {function} salt Optional async callback function.
 * @returns {string} A generated hash in format $id$salt$encrypted
 * @see https://en.wikipedia.org/wiki/Crypt_(C)
 */
var crypt3 = module.exports = function(key, salt, cb) {
	salt = salt || createSalt();
	if(cb) {
		return require('./async.js')(key, salt, cb);
	}
	return require('./sync.js')(key, salt);
};

/** Create salt 
 * @param {string} type The type of salt: md5, blowfish (only some linux distros), sha256 or sha512. Default is sha512.
 * @returns {string} Generated salt string
 */
crypt3.createSalt = createSalt;

/* EOF */
