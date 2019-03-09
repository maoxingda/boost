// ptree.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <atlbase.h>
// ----------------------------------------------------------------------------
// Copyright (C) 2002-2006 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

//[debug_settings_includes
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "boost/property_tree/json_parser.hpp"
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
namespace pt = boost::property_tree;
//]
//[debug_settings_data
struct debug_settings
{
	std::wstring m_file;               // log filename
	int m_level;                      // debug level
	std::set<std::wstring> m_modules;  // modules where logging is enabled
	void load(const std::string &filename);
	void save(const std::string &filename);

	//debug_settings()
	//	: m_file(L"日志")
	//	, m_level(4)
	//{
	//	m_modules.insert(L"模块1");
	//	m_modules.insert(L"模块2");
	//	m_modules.insert(L"模块3");
	//}
};
//]
//[debug_settings_load
#define U8_U16(u8str) CA2T(u8str, CP_UTF8)
#define U16_U8(u16str) CT2A(u16str, CP_UTF8)
void debug_settings::load(const std::string &filename)
{
	std::stringstream ss;
	ss << U16_U8(L"<?xml version=\"1.0\" encoding=\"utf-8\"?><debug><filename>日志</filename><level>4</level><modules><module>模块1</module><module>模块2</module><module>模块3</module></modules></debug>");

	// Create empty property tree object
	pt::ptree tree;

	// Parse the XML into the property tree.
	pt::read_xml(ss, tree);

	// Use the throwing version of get to find the debug filename.
	// If the path cannot be resolved, an exception is thrown.
	m_file = U8_U16(tree.get<std::string>("debug.filename").c_str());

	// Use the default-value version of get to find the debug level.
	// Note that the default value is used to deduce the target type.
	m_level = tree.get("debug.level", 0);

	// Use get_child to find the node containing the modules, and iterate over
	// its children. If the path cannot be resolved, get_child throws.
	// A C++11 for-range loop would also work.
	BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("debug.modules")) {
		// The data function is used to access the data stored in a node.
		m_modules.insert(U8_U16(v.second.data().c_str()));
	}
}
//]
//[debug_settings_save
void debug_settings::save(const std::string &filename)
{
	// Create an empty property tree object.
	pt::wptree tree;

	// Put the simple values into the tree. The integer is automatically
	// converted to a string. Note that the "debug" node is automatically
	// created if it doesn't exist.
	tree.put(L"debug.filename", CT2A(m_file.c_str(), CP_UTF8));
	tree.put(L"debug.level", m_level);

	// Add all the modules. Unlike put, which overwrites existing nodes, add
	// adds a new node at the lowest level, so the "modules" node will have
	// multiple "module" children.
	BOOST_FOREACH(const std::wstring &name, m_modules)
		tree.add(L"debug.modules.module", CT2A(name.c_str(), CP_UTF8));

	// Write property tree to XML file
	pt::write_xml(filename, tree);
}
//]
#include <locale>
int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		debug_settings ds;
		ds.load("debug_settings.xml");
		std::wcout.imbue(std::locale("chs"));
		std::wcout << ds.m_file << std::endl;
		std::wcout << ds.m_level << std::endl;
		//ds.save("debug_settings.xml");
	}
	catch (std::exception &e)
	{
		std::cout << "Error: " << e.what() << "\n";
	}
	return 0;
}

