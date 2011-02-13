# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

puts "="*80, "Setting Up Volnitsky Substring Search Algorithm Construction","="*80

# Give it a name
extension_name = 'volnitsky'

# The destination
dir_config(extension_name)

# Do the work
create_makefile(extension_name)