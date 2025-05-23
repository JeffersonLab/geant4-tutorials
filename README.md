---

This README documents how to edit the Hands-On material
and produce the content files that the users will consume.

To download, edit and update the repository, see the
[Working with the repository](#working-with-the-repository) section below.

---

## Structure

The sources are HTML files inside `HandsOnX` subdirs in the
`src` directory, each corresponding to a section.
An example is shown below:

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

> [!Important]
> Two files are **mandatory** in each `HandsOnX` directory:
> 1. `content.dat`: defines the order of the sections.
> 2. `navigation.html`: creates the table of contents that is shown as _topbar_
	 > to the users.

## Test the HTML files

The python script  `create_pages.py` is run by GitHub Actions
to create the website. It builds the HandsOnX directories 
containing the various `index.html`. 

To test it locally:

```
./create_pages.py
```

> [!Note]
> The created HandsOnX directories should not be added to this repository.

---

### Update Versions numbers and filenames

Upon a new Geant4 relase, update the version numbers and filenames in `versions.dat`.
An example one entry in `versions.dat` is:

```
###G4VERSION: "11.3.0"
```

so all instances of `###G4VERSION` in the HTML files will 
be replaced by that version.

> [!Important]
> **The HTML files should only contain the strings listed in `versions.dat` and no
hardcoded version numbers or filenames**

To add a new version string, add a new line to the `versions.dat` file and
re-run `create_pages.py`.

---

### Working with the repository

To clone the repository to your local machine (do this only once):

```
git clone https://github.com/JeffersonLab/geant4-tutorials-dev.git
cd geant4-tutorials
```

The following operations are done in the `geant4-tutorials` directory.

It's a good practice to update the local repository before starting to work,
to avoid conflicts with other developers:

```
git pull
```

To add a new file:

```
git add <filename>
```

To commit changes:

```
git commit -m "message"
```

where `message` is a short description of the changes.

To push changes to the repository:

```
git push
```

> [!Note]
> As of 2024, GitHub requires two-factor authentication
> for pushing changes to the repository.
> [Check their instructions](https://docs.github.com/en/authentication/securing-your-account-with-two-factor-authentication-2fa/configuring-two-factor-authentication)
> on how to set it up.

---

## Creating a "past" release of the tutorials

Create a new subdir in the `past_events` directory with the name of the event.
Copy the `tutorial` content to the new subdir. 
Add links in src/index.html and JeffersonLab/g4home/a_tutorials.md src/index.html

## Style and Navigation

The following files are used to define the style and navigation of the pages.

- `main.scss`: the main style file.
- `main.js`: the main javascript file.
- `header.html`: the header of the page.
- `footer.html`: the footer of the page.

These files should not be edited.

---

