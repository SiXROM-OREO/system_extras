/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>

#include <android-base/file.h>
#include <android-base/test_utils.h>

#include "command.h"
#include "get_test_data.h"

static std::unique_ptr<Command> ReportSampleCmd() {
  return CreateCommandInstance("report-sample");
}

TEST(cmd_report_sample, text) {
  ASSERT_TRUE(
      ReportSampleCmd()->Run({"-i", GetTestData(PERF_DATA_WITH_SYMBOLS)}));
}

TEST(cmd_report_sample, output_option) {
  TemporaryFile tmpfile;
  ASSERT_TRUE(ReportSampleCmd()->Run(
      {"-i", GetTestData(PERF_DATA_WITH_SYMBOLS), "-o", tmpfile.path}));
}

TEST(cmd_report_sample, show_callchain_option) {
  TemporaryFile tmpfile;
  ASSERT_TRUE(ReportSampleCmd()->Run({"-i", GetTestData(CALLGRAPH_FP_PERF_DATA),
                                      "-o", tmpfile.path, "--show-callchain"}));
}

TEST(cmd_report_sample, protobuf_option) {
  TemporaryFile tmpfile;
  TemporaryFile tmpfile2;
  ASSERT_TRUE(ReportSampleCmd()->Run({"-i", GetTestData(PERF_DATA_WITH_SYMBOLS),
                                      "-o", tmpfile.path, "--protobuf"}));
  ASSERT_TRUE(ReportSampleCmd()->Run(
      {"--dump-protobuf-report", tmpfile.path, "-o", tmpfile2.path}));
  std::string data;
  ASSERT_TRUE(android::base::ReadFileToString(tmpfile2.path, &data));
  ASSERT_NE(data.find("file:"), std::string::npos);
}
