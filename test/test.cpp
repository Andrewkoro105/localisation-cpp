#include <gtest/gtest.h>
#include <localisation/system.hpp>

TEST(load_from_file, for_now_language){
	loc::system.clear();
	loc::system.load_from_directory("../test");
	loc::system.set_now_language("ru");
	ASSERT_EQ(loc::system.get_text("t"), "тест");
	ASSERT_EQ(loc::system.get_text("t2"), "тест2");
	loc::system.set_now_language("en");
	ASSERT_EQ(loc::system.get_text("t"), "test");
	ASSERT_EQ(loc::system.get_text("t2"), "test2");
}

TEST(load_from_file, for_default_language){
	loc::system.clear();
	loc::system.load_from_directory("../test");
	loc::system.set_default_language("ru");
	loc::system.set_now_language("jp");
	ASSERT_EQ(loc::system.get_text("t"), "тест");
	ASSERT_EQ(loc::system.get_text("t2"), "тест2");
	loc::system.set_default_language("en");
	loc::system.set_now_language("jp");
	ASSERT_EQ(loc::system.get_text("t"), "test");
	ASSERT_EQ(loc::system.get_text("t2"), "test2");
}

TEST(load_from_dir, base){
	loc::system.clear();
	loc::system.load_from_directory("../test");
	loc::system.set_now_language("en");
	ASSERT_EQ(loc::system.get_text("t3"), "test3");
	ASSERT_EQ(loc::system.get_text("t4"), "test4");
}

TEST(load_from_module, base){
	loc::system.clear();
	loc::system.load_from_directory("../test");
	loc::system.set_now_language("en");
	try {
		loc::system.get_text("t5");
		ASSERT_TRUE(false);
	} catch(...) {
		ASSERT_TRUE(true);
	}
	loc::system.clear();
	loc::system.set_modules({"_deps/localizationzip-build/libLocalizationZip"});
	loc::system.load_from_directory("../test");
	loc::system.set_now_language("en");
	ASSERT_EQ(loc::system.get_text("t5"), "test5");
}