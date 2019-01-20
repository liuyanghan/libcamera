/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * libcamera V4L2 API tests
 */

#include <iostream>

#include "v4l2_device_test.h"

namespace {

class DoubleOpen : public V4L2DeviceTest
{
protected:
	int run()
	{
		int ret;

		/*
		 * Expect failure: The device has already been opened by the
		 * V4L2DeviceTest base class
		 */
		ret = dev_->open();
		if (!ret) {
			std::cout << "Double open erroneously succeeded" << std::endl;
			dev_->close();
			return TestFail;
		}

		return TestPass;
	}
};

} /* namespace */

TEST_REGISTER(DoubleOpen);