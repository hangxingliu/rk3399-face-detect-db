# Ctags Utils

[Exuberant Ctags](http://ctags.sourceforge.net/) has been deprecated.
So I chose [Universal-ctags](https://ctags.io/), a continue the development of Exuberant Ctags.

## How to install `ctags`

```
git clone https://github.com/universal-ctags/ctags.git
cd ctags
./autogen.sh
./configure --prefix=/where/you/want # defaults to /usr/local
make
sudo make install
```

## How to use `ctags`

Remember: `ctags --help`

Example:

```
ctags -o - --output-format=xref filename.cc
```
