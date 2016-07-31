/** Node.js Crypt(3) Library */

var salters = {
	'md5': function() { return '$1$'+require('crypto').randomBytes(10).toString('base64'); },
	'blowfish': function() { return '$2a$'+require('crypto').randomBytes(10).toString('base64'); },
	'sha256': function() { return '$5$'+require('crypto').randomBytes(10).toString('base64'); },
	'sha512': function() { return '$6$'+require('crypto').randomBytes(10).toString('base64'); }
};

function createSalt(type) {
	type = type || 'sha512';
	if(!salters[type]) throw new TypeError('Unknown salt type at crypt3.createSalt: ' + type);
	return salters[type]();
};

createSalt.salters = salters;

/** Create salt 
 * @param {string} type The type of salt: md5, blowfish (only some linux distros), sha256 or sha512. Default is sha512.
 * @returns {string} Generated salt string
 */
module.exports = createSalt;

/* EOF */
