require 'bundler/setup'
require "bundler/gem_tasks"
Bundler.require(:development)

require "rake/extensiontask"
require "rake/testtask"
require "rubygems/package_task"
require 'fileutils'

spec = Gem::Specification.load("jaguar.gemspec")

# Setup compile tasks
Rake::ExtensionTask.new do |ext|
  ext.gem_spec = spec
  ext.name = 'jaguar/http/parser'
  ext.ext_dir = 'ext/jaguar'
  ext.lib_dir = 'lib'
  ext.config_options << '--coverage' if ENV['COVERALLS_REPO_TOKEN']
end

task :console => :compile do
  exec 'irb -I lib -r jaguar/http/parser'
end
task :c => :console
