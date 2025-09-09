setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
  valgrind="valgrind --leak-check=full --error-exitcode=1"
}

@test "tempo describe runs without memory problem" {
  $valgrind tempo describe < "$examples_dir/6.ts"
}

@test "tempo help runs without memory problem" {
  $valgrind tempo help
}

@test "tempo show runs without memory problem" {
  $valgrind tempo show < "$examples_dir/6.ts"
}
