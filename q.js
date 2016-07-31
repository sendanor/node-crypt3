/* Node.js Crypt(3) Library -- Async version */

var createSalt = require('./createsalt.js');
var crypt3async = require('./async.js');
var _Q = require('q');

/** Crypt(3) password and data encryption.
 * @param {string} key user's typed password
 * @param {string} salt Optional salt, for example SHA-512 use "$6$salt$". Please note: SHA-512 only works on glibc2 systems.
 * @param {function} callback The callback function, for example `function(err, hash)` where `hash` is {string} A generated hash in format `$id$salt$encrypted`
 * @see https://en.wikipedia.org/wiki/Crypt_(C)
 */
var crypt3 = module.exports = function(key, salt) {
	salt = salt || createSalt();
	var defer = _Q.defer();
	crypt3async(key, salt, function(err, value) {
		if(err) {
			defer.reject(err);
		} else {
			defer.resolve(value);
		}
	});
	return defer.promise;
};

/** Create salt 
 * @param {string} type The type of salt: md5, blowfish (only some linux distros), sha256 or sha512. Default is sha512.
 * @returns {string} Generated salt string
 */
crypt3.createSalt = createSalt;

/* EOF */
