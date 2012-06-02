require 'mkmf'
require 'ruby_core_source'

def add_define(name)
  $defs.push("-D#{name}")
end

hdrs = proc {
  have_header('vm_core.h')
  have_header('iseq.h')
}
major, minor, patch = RUBY_VERSION.split('.').map!(&:to_i)
add_define 'RUBY19'
add_define 'RUBY193' if patch >= 3

Ruby_core_source::create_makefile_with_core(hdrs, 'stacktrace/stacktrace')
