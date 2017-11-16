Gem::Specification.new do |s|
  s.name        = 'Jaguar'
  s.version     = '0.1'
  s.licenses    = ['MIT']
  s.authors     = ['Jon Bracy']
  s.email       = ['jonbracy@gmail.com']
  s.homepage    = 'https://github.com/malomalo/jaguar'
  s.summary     = ''
  s.description = ''

  # s.extra_rdoc_files = %w(README.md)
  # s.rdoc_options.concat ['--main', 'README.md']

  s.files = `git ls-files`.split("\n")
  s.extensions = ['ext/jaguar/extconf.rb']
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.require_paths = ['lib']

  s.required_ruby_version = '>= 2.0.0'

  s.add_development_dependency 'rake-compiler',       '~> 1.0.4'
end
