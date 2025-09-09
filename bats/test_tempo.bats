setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Normal usage
# ------------

@test "tempo help shows the help" {
  skip
  run tempo help
  assert_line --index 0 "Usage: tempo SUBCOMMAND"
  assert_line --index 1 "Displays information about a timeseries."
}

@test "tempo help mentions the describe subcommand" {
  skip
  run tempo help
  assert_line --partial "  describe:"
}

@test "tempo help mentions the help subcommand" {
  skip
  run tempo help
  assert_line --partial "  help:"
}

@test "tempo help mentions the show subcommand" {
  skip
  run tempo help
  assert_line --partial "  show:"
}

# Wrong usage
# -----------

@test "tempo without subcommand returns an error" {
  skip
  run tempo
  [ "$status" -eq 1 ]
  assert_line --index 0 "error: subcommand is mandatory"
}

@test "tempo with unrecognized subcommand returns an error" {
  skip
  run tempo sc
  [ "$status" -eq 1 ]
  assert_line --index 0 "error: unrecognized subcommand 'sc'"
}
