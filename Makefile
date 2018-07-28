install:
	  -/usr/bin/yes | pip uninstall h3py
	  @rm -rf build && python setup.py install

test:
		@cd h3py && python -m pytest -sv --cov=. tests/ --cov-report=term --cache-clear && cd ..

h3-clone:
	  git clone https://github.com/uber/h3.git h3py/h3

h3-build:
	  @cd h3py/h3 && cmake . && make && cd ../..

h3-remove:
	  rm -rf h3py/h3/

install-clean: h3-remove h3-clone  h3-build install
	  rm -rf h3py/tests/inputfiles && cp -r h3py/h3/tests/inputfiles h3py/tests/.
