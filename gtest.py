
from waflib.TaskGen import feature, before, after
from waflib import Task, Utils


def configure (conf):
	conf.check_cxx (lib='pthread', define_name='GTEST_HAS_PTHREAD', uselib_store='GTEST_PTHREAD')


@feature ('gtest')
@before ('process_source')
def gtest_link (self):
	self.use = self.to_list (getattr(self, 'use', [])) + ['GTEST_PTHREAD']


@feature ('gtest')
@after ('apply_link')
def run_test (self):
	if getattr (self, 'link_task', None):
		self.create_task ('gtest_unit', self.link_task.outputs)


class gtest_unit (Task.Task):
	color = 'PINK'
	after = ['vnum', 'inst']
	vars = []
	
	def run (self):
		exec_path = self.inputs[0].abspath()
		Utils.subprocess.call (exec_path)