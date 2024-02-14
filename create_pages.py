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
print("Current directory: ", current_dir)

# Get the parent directory
parent_dir = os.path.dirname(current_dir)
print("Parent directory: ", parent_dir)

# Get the list of subdirectories starting with HandsOn
subdirs = [d for d in os.listdir(current_dir) if os.path.isdir(d) and d.startswith("HandsOn1")]

# Get the content of header.html and footer.html
header_file = os.path.join(parent_dir, "header.html")
footer_file = os.path.join(parent_dir, "footer.html")
with open(header_file, "r") as file:
	header = file.read()
with open(footer_file, "r") as file:
	footer = file.read()

# Add the header and footer to the content of index.html in each subdirectory
for subdir in subdirs:
	index_file = os.path.join(current_dir, subdir, "index.html")
	with open(index_file, "w") as file:
		file.write(header)
		file.write(footer)

# Loop over the subdirectories
for subdir in subdirs:
	# Get the content of content.dat
	content_file = os.path.join(current_dir, subdir, "content.dat")
	with open(content_file, "r") as file:
		content = file.read()

	# loop over each line of content.dat
	content_list = content.split("\n")
	print("Content list: ", content_list)

