
var assert = require('assert');

/* Test sync version */
var crypt3 = require('../');

assert.throws(function() {
	crypt3()
}, /Wrong arguments/)

assert.equal(crypt3('pass', 'salt'), 'sa5JEXtYx/rm6')
assert.equal(crypt3('pass', 'sa5JEXtYx/rm6'), 'sa5JEXtYx/rm6')

var hash = crypt3('password')
assert.equal(crypt3('password', hash), hash)
assert.notEqual(crypt3('bad-pass', hash), hash)

/* Test async version */
var crypt3a = require('../async.js');

assert.throws(function() {
	crypt3a()
}, /Wrong arguments/);

crypt3a('pass', 'salt', function(err, value) {
	if(err) { throw new TypeError(""+err); }
	assert.equal(value, 'sa5JEXtYx/rm6')
});

crypt3a('pass', 'sa5JEXtYx/rm6', function(err, value) {
	if(err) { throw new TypeError(""+err); }
	assert.equal(value, 'sa5JEXtYx/rm6')
});

crypt3a('password', undefined, function(err, hash) {

	crypt3a('password', hash, function(err, value) {
		if(err) { throw new TypeError(""+err); }
		assert.equal(value, hash);
	});

	crypt3a('bad-pass', hash, function(err, value) {
		if(err) { throw new TypeError(""+err); }
		assert.notEqual(value, hash);
	});
});

/* Test q version */
var _Q = require('q');
var crypt3q = require('../q.js');

assert.throws(function() {
	crypt3q()
}, /Wrong arguments/);

_Q.fcall(function() {
	return crypt3q('pass', 'salt').then(function(value) {
		assert.equal(value, 'sa5JEXtYx/rm6');
	});
}).then(function() {
	return crypt3q('pass', 'sa5JEXtYx/rm6').then(function(value) {
		assert.equal(value, 'sa5JEXtYx/rm6');
	});
}).then(function() {
	return crypt3q('password').then(function(hash) {
		return crypt3q('password', hash).then(function(value) {
			assert.equal(value, hash);
		}).then(function() {
			return crypt3q('bad-pass', hash).then(function(value) {
				assert.notEqual(value, hash);
			});
		});
	});
}).then(function() {
	console.log('All promise-based tests OK');
}).fail(function(err) {
	if(err.stack) {
		console.log(err.stack);
	} else {
		console.log('Error: ' + err);
	}
}).done();



