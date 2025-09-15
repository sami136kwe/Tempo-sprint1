setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Wrong datetime
# --------------

@test "tempo show with datetime missing the character T returns an error" {
  skip
  run tempo show < "$examples_dir/missing_t.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid datetime format (should be YYYY-mm-DDTHH:MM:SS)"
}

@test "tempo show with wrong character in date returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_char_date.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid datetime format (should be YYYY-mm-DDTHH:MM:SS)"
}

@test "tempo show with wrong character in time returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_char_time.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid datetime format (should be YYYY-mm-DDTHH:MM:SS)"
}

@test "tempo show with wrong day returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_day.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid datetime (2025-02-29T00:00:00)"
}

@test "tempo show with wrong month returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_month.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid datetime (2025-23-04T00:00:00)"
}

@test "tempo show with wrong year returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_year.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid datetime (5002-06-04T00:00:00)"
}

# Wrong observation
# -----------------

@test "tempo show with wrong character in observation returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_char_observation.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid observation format (0:10)"
}

@test "tempo show with wrong offset format returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_offset.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid observation format (0m 10)"
}

@test "tempo show with wrong value format returns an error" {
  skip
  run tempo show < "$examples_dir/wrong_value.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid observation format (0 10.2)"
}

@test "tempo show with negative offset format returns an error" {
  skip
  run tempo show < "$examples_dir/negative_offset.invalid"
  [ "$status" -eq 2 ]
  assert_output "error: invalid offset (-30 10)"
}
