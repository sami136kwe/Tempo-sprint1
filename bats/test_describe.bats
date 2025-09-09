setup() {
  load 'bats-support/load'
  load 'bats-assert/load'
  root_dir="$(cd "$( dirname "$BATS_TEST_FILENAME" )/.." >/dev/null 2>&1 && pwd)"
  PATH="$root_dir/bin:$PATH"
  examples_dir="$root_dir/examples"
}

# Domain
# ------

@test "tempo describe with empty timeseries displays the domain" {
  skip
  run tempo describe < "$examples_dir/empty.ts"
  assert_line "Domain: [2025-09-01T09:00:00, 2025-09-01T09:00:00]"
}

@test "tempo describe with timeseries of size 1 displays the domain" {
  skip
  run tempo describe < "$examples_dir/1.ts"
  assert_line "Domain: [2025-09-01T09:00:00, 2025-09-01T09:00:00]"
}

@test "tempo describe with chronological timeseries of size 2 displays the domain" {
  skip
  run tempo describe < "$examples_dir/2.ts"
  assert_line "Domain: [2025-09-01T09:00:00, 2025-09-01T10:00:00]"
}

@test "tempo describe with antichronological timeseries of size 2 displays the domain" {
  skip
  run tempo describe < "$examples_dir/2rev.ts"
  assert_line "Domain: [2025-09-01T09:00:00, 2025-09-01T10:00:00]"
}

@test "tempo describe with timeseries with repeated offsets displays the domain" {
  skip
  run tempo describe < "$examples_dir/2dbl.ts"
  assert_line "Domain: [2025-09-01T09:00:00, 2025-09-01T09:00:00]"
}

# Codomain
# --------

@test "tempo describe with empty timeseries displays the codomain" {
  skip
  run tempo describe < "$examples_dir/empty.ts"
  assert_line "Codomain: none"
}

@test "tempo describe with timeseries of size 1 displays the codomain" {
  skip
  run tempo describe < "$examples_dir/1.ts"
  assert_line "Codomain: [10, 10]"
}

@test "tempo describe with chronological timeseries of size 2 displays the codomain" {
  skip
  run tempo describe < "$examples_dir/2.ts"
  assert_line "Codomain: [10, 20]"
}

@test "tempo describe with antichronological timeseries of size 2 displays the codomain" {
  skip
  run tempo describe < "$examples_dir/2rev.ts"
  assert_line "Codomain: [10, 20]"
}

@test "tempo describe with timeseries with repeated offsets displays the codomain" {
  skip
  run tempo describe < "$examples_dir/2dbl.ts"
  assert_line "Codomain: [20, 20]"
}

# Size
# ----

@test "tempo describe with empty timeseries displays the size" {
  skip
  run tempo describe < "$examples_dir/empty.ts"
  assert_line "Size: 0"
}

@test "tempo describe with timeseries of size 1 displays the size" {
  skip
  run tempo describe < "$examples_dir/1.ts"
  assert_line "Size: 1"
}

@test "tempo describe with chronological timeseries of size 2 displays the size" {
  skip
  run tempo describe < "$examples_dir/2.ts"
  assert_line "Size: 2"
}

@test "tempo describe with antichronological timeseries of size 2 displays the size" {
  skip
  run tempo describe < "$examples_dir/2rev.ts"
  assert_line "Size: 2"
}

@test "tempo describe with timeseries with repeated offsets displays the size" {
  skip
  run tempo describe < "$examples_dir/2dbl.ts"
  assert_line "Size: 1"
}

# Duration
# --------

@test "tempo describe with empty timeseries displays the duration" {
  skip
  run tempo describe < "$examples_dir/empty.ts"
  assert_line "Duration: 0"
}

@test "tempo describe with timeseries of size 1 displays the duration" {
  skip
  run tempo describe < "$examples_dir/1.ts"
  assert_line "Duration: 0"
}

@test "tempo describe with chronological timeseries of size 2 displays the duration" {
  skip
  run tempo describe < "$examples_dir/2.ts"
  assert_line "Duration: 3600"
}

@test "tempo describe with antichronological timeseries of size 2 displays the duration" {
  skip
  run tempo describe < "$examples_dir/2rev.ts"
  assert_line "Duration: 3600"
}

@test "tempo describe with timeseries with repeated offsets displays the duration" {
  skip
  run tempo describe < "$examples_dir/2dbl.ts"
  assert_line "Duration: 0"
}

# Amplitude
# ---------

@test "tempo describe with empty timeseries displays the amplitude" {
  skip
  run tempo describe < "$examples_dir/empty.ts"
  assert_line "Amplitude: 0"
}

@test "tempo describe with timeseries of size 1 displays the amplitude" {
  skip
  run tempo describe < "$examples_dir/1.ts"
  assert_line "Amplitude: 0"
}

@test "tempo describe with chronological timeseries of size 2 displays the amplitude" {
  skip
  run tempo describe < "$examples_dir/2.ts"
  assert_line "Amplitude: 10"
}

@test "tempo describe with antichronological timeseries of size 2 displays the amplitude" {
  skip
  run tempo describe < "$examples_dir/2rev.ts"
  assert_line "Amplitude: 10"
}

@test "tempo describe with timeseries with repeated offsets displays the amplitude" {
  skip
  run tempo describe < "$examples_dir/2dbl.ts"
  assert_line "Amplitude: 0"
}
