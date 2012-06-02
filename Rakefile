# -*- ruby -*-

require 'rubygems'
require 'hoe'
require 'rake/extensiontask'

# Hoe.plugin :compiler
# Hoe.plugin :gem_prelude_sucks
# Hoe.plugin :inline
# Hoe.plugin :racc
# Hoe.plugin :rcov
# Hoe.plugin :rubyforge

Hoe.spec 'stacktrace' do
  developer('Sam Saffron', 'sam.saffron@gmail.com')

  self.rubyforge_name = 'stacktrace' # if different than 'stacktrace'
  self.extra_dev_deps << ['rake-compiler', '>=0']
  self.spec_extras = {:extensions => ["ext/stacktrace/extconf.rb"]}

  Rake::ExtensionTask.new('stacktrace', spec) do |ext|
    ext.lib_dir = File.join('lib', 'stacktrace')
  end
end

Rake::Task[:test].prerequisites << :compile

# vim: syntax=ruby
