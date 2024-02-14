#!/usr/bin/env python3

# Purposes:
# Create a index.html file for each HandsOn subdirectory
# In each HandsOn subdirectory, index.html is created with:
# - the content of common.html, a file in the parent directory
# - the content of the files listed in content.dat, a file in the subdirectory


import os


g4varsion = "11.2.0"

# Get the current directory
current_dir = os.getcwd()
print(" > Current directory: ", current_dir)


# Get the list of subdirectories starting with HandsOn
subdirs = [d for d in os.listdir(current_dir) if os.path.isdir(d) and d.startswith("HandsOn1")]

# Get the content of header.html and footer.html
header_file = os.path.join(current_dir, "header.html")
footer_file = os.path.join(current_dir, "footer.html")
with open(header_file, "r") as file:
	header = file.read()
with open(footer_file, "r") as file:
	footer = file.read()

# Add the header and footer to the content of index.html in each subdirectory
for subdir in subdirs:
	print("\n > Subdir: ", subdir)
	index_file = os.path.join(current_dir, subdir, "index.html")
	with open(index_file, "w") as file:
		file.write(header)

		navigation_file = os.path.join(current_dir, subdir, "navigation.html")
		with open(navigation_file, "r") as nav_file:
			navigation = nav_file.read()
			file.write(navigation)

		content_file = os.path.join( current_dir, subdir, "content.dat" )
		with open( content_file, "r" ) as subdir_file:
			content = subdir_file.read()

		# loop over each line of content.dat
		content_list = content.split( "\n" )

		# write the content of each file listed in content.dat to index.html
		for file_name in content_list:
			if file_name != "":
				file_path = os.path.join( current_dir, subdir, file_name )
				print("   > Adding: ", file_path )
				with open( file_path, "r" ) as subdir_file:
					file_content = subdir_file.read()
					file.write( file_content )
					# add a line break after each file
					file.write( "<br/><hr/>" )

		file.write(footer)

print("\n > Done!\n")