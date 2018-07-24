install:
	  -/usr/bin/yes | pip uninstall h3py
	  @rm -rf build && python setup.py install

test:
		@python -m pytest --cov=. tests/ --cov-report=term --cache-clear

h3-clone:
	  git clone https://github.com/uber/h3.git

h3-build:
	  @cd h3 && cmake . && make && cd ..

h3-remove:
	  rm -rf h3/

install-clean: h3-remove h3-clone  h3-build install
	  rm -rf tests/inputfiles && cp -r h3/tests/inputfiles tests/.
