import sys
sys.path.insert (0, '.')

from waflib.Build import BuildContext


top = '.'
out = 'build'


class TestContext (BuildContext):
	'''run the test suite'''
	cmd = 'test'
	fun = 'test'


def options (opt):
	opt.add_option ('--enable-tests', action='store_true', default=True, help='Build the test suite')

	opt.recurse ('src')
	opt.recurse ('test')


def configure (conf):
	conf.recurse ('src')
	if conf.options.enable_tests: conf.recurse ('test')


def build (bld):
	bld.recurse ('src')
	if bld.options.enable_tests: bld.recurse ('test')


def test (ctx):
	if ctx.options.enable_tests: ctx.recurse ("test")