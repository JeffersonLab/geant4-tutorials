#!/usr/bin/env python3

# Purposes:
# Create a index.html file for each HandsOn subdirectory
# In each HandsOn subdirectory, index.html is created with:
# - the content of common.html, a file in the parent directory
# - the content of the files listed in content.dat, a file in the subdirectory


import os
import shutil

# Generate the main.css file from the main.scss file
os.system("sass main.scss main.css")
os.system("rm main.css.map")

# reads the content of versions.dat
# and assign the values to a map
print("\n > Reading versions.dat\n")
versions_file = os.path.join(os.getcwd(), "versions.dat")
versions_map = {}
with open(versions_file, "r") as file:
	for line in file:
		line = line.strip()
		if line != "":
			# split the line at the first colon
			key, value = line.split(":", 1)
			# remove the leading and trailing whitespaces
			key = key.strip()
			value = value.strip()
			# remove the double quotes
			value = value.replace('"', '')
			versions_map[key] = value
			print(" > ", key, ":", value)

# Get the current directory
current_dir = os.getcwd()

# Create the tutorial directory
deploy_dir = os.path.join(current_dir, "tutorial")
if os.path.exists(deploy_dir):
	shutil.rmtree(deploy_dir)
os.makedirs(deploy_dir)

# Get the content of header.html and footer.html
header_file = os.path.join(current_dir, "header.html")
footer_file = os.path.join(current_dir, "footer.html")
with open(header_file, "r") as file:
	header = file.read()
with open(footer_file, "r") as file:
	footer = file.read()

# Get the list of subdirectories in the subdirectory 'src'
src = os.path.join(current_dir, "src")
sub_dirs = [d for d in os.listdir(src) if d.startswith("HandsOn")]
print("\n > Subdirectories to be published: ", sub_dirs)

# move the main.* to the deploy directory
shutil.copy("main.css", deploy_dir)
shutil.copy("main.js", deploy_dir)
shutil.copy("src/index.html", deploy_dir)
shutil.copy("src/index_devel.html", deploy_dir)
print("\n > Coontent of tutorial directory: ", os.listdir(deploy_dir))

# Add the header and footer to the content of index.html in each subdirectory
for subdir in sub_dirs:
	# create subdir if it does not exist
	deploy_subdir = os.path.join(deploy_dir, subdir)
	if not os.path.exists(deploy_subdir):
		os.makedirs(deploy_subdir)

	print("\n > Source Subdir: ", subdir)
	src_dir = os.path.join(src, subdir)

	#  Copy PNG, GIF, root macros and python files
	image_files = [file for file in os.listdir(src_dir) if
	               file.endswith('.png') or file.endswith('.gif') or file.endswith(
		               '.C') or file.endswith('.py')]
	for file in image_files:
		source_path = os.path.join(src_dir, file)
		destination_path = os.path.join(deploy_subdir, file)
		shutil.copy(source_path, destination_path)
	print(f"   > Copied {image_files} to {deploy_subdir}")

	index_file = os.path.join(current_dir, deploy_subdir, "index.html")
	with open(index_file, "w") as file:
		file.write(header)

		navigation_file = os.path.join(src, subdir, "navigation.html")
		with open(navigation_file, "r") as nav_file:
			navigation = nav_file.read()
			file.write(navigation)

		content_file = os.path.join(src, subdir, "content.dat")
		with open(content_file, "r") as subdir_file:
			content = subdir_file.read()

		# loop over each line of content.dat
		content_list = content.split("\n")

		# write the content of each file listed in content.dat to index.html
		for file_name in content_list:
			if file_name != "":
				file_path = os.path.join(src, subdir, file_name)
				print("   > Adding: ", file_path)
				with open(file_path, "r") as subdir_file:
					file_content = subdir_file.read()
					file.write(file_content)
					# add a line break after each file
					file.write("<br/><hr/>")

		file.write(footer)

	# now replacing string versions
	with open(index_file, "r") as file:
		filedata = file.read()

		filedata = filedata.replace("###HANDSONVERSION", subdir)
		# replace each key in the map with its value
		for key in versions_map:
			filedata = filedata.replace(key, versions_map[key])

	with open(index_file, "w") as file:
		file.write(filedata)

# create a tar ball for each subdirectory in ho_materials and move it to tutorial
ho_materials = os.path.join(current_dir, "ho_materials")
if os.path.exists(ho_materials):
	ho_materials_dirs = [tdir for tdir in os.listdir(ho_materials) if
	                     os.path.isdir(os.path.join(ho_materials, tdir))]
	ho_materials_dirs.sort()
	print(f"\n> Creating tar balls for {ho_materials_dirs}")
	for dir_to_be_zipped in ho_materials_dirs:
		tar_command = "cd " + ho_materials + ";"
		tar_command += " tar czf " + "../tutorial/" + dir_to_be_zipped + ".tar.gz " + dir_to_be_zipped + ";"
		tar_command += " cd " + current_dir
		os.system(tar_command)

# copy past_events directoru to tutorial
past_events = os.path.join(current_dir, "past_events")
if os.path.exists(past_events):
	shutil.copytree(past_events, os.path.join(deploy_dir, "past_events"))

print("\n > Done!\n")
