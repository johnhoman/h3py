install:
	  rm -rf build && python setup.py install

test:
	  python -m unittest discover

h3-clone:
	  git clone https://github.com/uber/h3.git

h3-build:
	  cd h3 && cmake . && make && cd ..

h3-remove:
	  rm -rf h3/

install-clean: h3-remove h3-clone  h3-build install
	  rm -rf tests/inputfiles && cp -r h3/tests/inputfiles tests/.
