
var assert = require('better-assert');
var Sophist = require('..');

describe('Sophist#getSync', function () {
  var db;

  before(function (done) {
    db = new Sophist('./testdb');
    db.open(done);
  });

  beforeEach(function (done) {
    db.clear(done);
  });

  after(function (done) {
    db.close(done);
  });

  it('should get a value', function () {
    db.setSync('foo', 'bar');
    assert('bar' == db.getSync('foo'));
  });

  it('should provide null for non-existant keys', function () {
    assert(null === db.getSync('blahblah'))
  });
});
