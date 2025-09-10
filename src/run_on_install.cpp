#include <string>
#include <syscall.h>

int run_on_install();

const std::string SCRIPTS_DIR = "./scripts/";
const std::string PYTHON = "python ";

int run_on_install() {
  system((PYTHON + SCRIPTS_DIR + "dict-stripper.py").c_str());
  system((PYTHON + SCRIPTS_DIR + "frequency_tables.py").c_str());

  return 0;
}
