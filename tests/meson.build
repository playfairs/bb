if get_option('tests')
  foreach test_name : ['test_filesize', 'test_pattern', 'test_random', 'test_args']
    bb_test = executable(
      test_name,
      [test_name + '.cpp'],
      include_directories: inc,
      link_with: bb_lib,
      cpp_args: cpp_args,
    )

    test(test_name, bb_test)
  endforeach
endif
