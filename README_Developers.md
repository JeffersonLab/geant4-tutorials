


**This documentation is for Geant4 developers only.** 

This README documents how to develop the Hands-On material 
and produce the `index.html` file that the users will consume.

To download, edit and update the repository, see the 
[Working with the repository](#working-with-the-repository) section below.


### Structure

The sources are HTML files inside `HandsOnX` subdirs in the 
`src` directory, each corresponding to a section. 
An example for HandsOn1 is shown below:

```
src/HandsOn1
├── content.dat
├── convention.html
├── docker.html
├── first_example.html
├── geant4_application.html
├── geant4_compilation.html
├── geant4_installation.html
├── introduction.html
├── navigation.html
├── pre_compiled.html
└── vm.html
```

Two files are **mandatory** in each `HandsOnX` directory:

1. `content.dat`: defines the order of the sections.
2. `navigation.html`: creates the table of contents that is shown as _topbar_
to the users.

Once the files are edited, run the python script  `create_pages.py`  
to produce the `index.html` files in the HandsOnX directories:

```python create_pages.py```



### Versions numbers and filenames

`create_pages.py` uses the `versions.dat` file to define 
strings to be replaced in the HTML files.
For example if the `versions.dat` file contains:

`
###G4VERSION: "11.2.0"
`

all instances of `###G4VERSION` in the HTML files will be replaced by `11.2.0`.

> The HTML files should only contain the strings listed in `versions.dat` and no 
hardcoded version numbers or filenames

To add a new version string, add a new line to the `versions.dat` file and 
re-run `create_pages.py`.



### Style and Navigation

The following files are used to define the style and navigation of the pages. 

- `main.scss`: the main style file.
- `main.js`: the main javascript file.
- `header.html`: the header of the page.
- `footer.html`: the footer of the page.

> These files should not be edited.


### Working with the repository

To clone the repository to your local machine (only once):

```
git clone https://github.com/JeffersonLab/geant4-tutorials.git
cd geant4-tutorials
```

The following operations are done in the `geant4-tutorials` directory.

It's a good practice to update the local repository before starting to work,
to avoid conflicts with other developers:

```git pull```

To add a new file:

```git add <filename>```

To commit changes:

```git commit -m "message"```

where `message` is a short description of the changes.

To push changes to the repository:

```git push```

