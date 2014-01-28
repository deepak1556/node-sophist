var Sophist = require('..');
var assert = require('better-assert');

describe('Sophist#readStream', function () {

  var db;

  beforeEach(function (done) {
    db = new Sophist('./testdb');
    db.open(function (err) {
      if (err) return done(err);
      db.clear(done);
    });
  });

  afterEach(function (done) {
    db.close(done);
  });

  it('should emit null', function (done) {
  	var readStream = db.createReadStream();
  	readStream.on('data', function(data) {
  	  assert(null === data);
  	  done();
  	}).on('error', function(err) {
  	  done(err);
  	});
  });

  it('should emit data', function (done) {
  	db.set('foo', 'bar', function(err) {
  	  if(err) return done(err);
  	  var readStream = db.createReadStream();
  	  readStream.on('data', function(data) {
  	    assert("foo" === data.key);
  	    done();
  	  }).on('error', function(err) {
  	    done(err);
  	  });
  	});  
  });

});