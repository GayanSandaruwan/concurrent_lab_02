Compiling each source code.

gcc -o name_of_the_file_to_be_created name_of_source_file -lpthread

running each compiled file

for Serial ---- ./name_of_serial_file number_of_nodes number_of_operations fraction_for_member fraction_for_insert fraction_for_delete

for One mutex ---- ./name_of_one_mutex_file number_of_nodes number_of_operations fraction_for_member fraction_for_insert fraction_for_delete number_of_threads

for Read Write ---- ./name_of_read_write_file number_of_nodes number_of_operations fraction_for_member fraction_for_insert fraction_for_delete number_of_threads


