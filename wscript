import sys
sys.path.insert (0, '.')


top = '.'
out = 'build'


def options (opt):
	opt.recurse ('src')
	opt.recurse ('test')

def configure (conf):
	conf.recurse ('src')
	conf.recurse ('test')

def build (bld):
	bld.recurse ('src')
	bld.recurse ('test')
