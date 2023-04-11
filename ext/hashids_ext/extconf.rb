require "mkmf"
require "fileutils"
require "pathname"

hashids_c_src_dir = Pathname(File.expand_path('../../../hashids.c/src', __FILE__))
hashids_c_files = %w[hashids.c hashids.h].map { |f| hashids_c_src_dir.join(f) }
hashids_ext_dir = Pathname(File.expand_path('..', __FILE__))
hashids_c_files.each do |f|
  FileUtils.rm_f(hashids_ext_dir.join(File.basename(f)))
  FileUtils.symlink(f, hashids_ext_dir.join(File.basename(f)))
end

create_makefile("hashids_ext/hashids_ext")
