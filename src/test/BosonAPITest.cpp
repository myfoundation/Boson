
#include "BosonAPITest.h"

using namespace Boson;



BosonAPITest::BosonAPITest(char* path) {
	//std::remove(path);
	db.open(path, false);
}


BosonAPITest::~BosonAPITest() {
	db.close();
}



void BosonAPITest::insertData() {
	std::cout << "============================================================================================" << std::endl;
	std::cout << "INSERTING\n";
	std::cout << "============================================================================================" << std::endl;

	for (int i = 0; i < 100; i++) {
		db.insert("Anyways all project will be completed.");
		db.insert("Beniki told me that message length is just a marker of records in the storage file.");
		db.insert("Syely lovely touches his hairs.");
		db.insert("Vereniki died trying to get rich.");
		db.insert("Thats what happened.");
		db.insert("Anyways all project will be completed.");
		db.insert("Beniki told me that message length is just a marker of records in the storage file.");
		db.insert("Unlikely that it would happen");
		db.insert("But still believa I can do it");
		db.insert("lets try again to create more nodes");
		db.insert("And test overflow cases");
	}
}


void BosonAPITest::eraseData() {

	std::cout << "============================================================================================" << std::endl;
	std::cout << "DELETING\n";
	std::cout << "============================================================================================" << std::endl;

	uint64_t M = db.size();
	for (uint64_t i = 0; i < M; i++) {
		db.erase(i);
	}

	db.getCacheHits();

}


void BosonAPITest::traverseEntries() {


	std::cout << "============================================================================================" << std::endl;
	std::cout << "TRAVERSING ENTRIES\n";
	std::cout << "============================================================================================" << std::endl;

	uint64_t key = 0;
	//uint64_t lastKey = ;
	//auto value = db.get(key);
	auto pair = db.first(); // std::make_pair(key, value);
	auto value = pair.second;

	// traverse entries
	if (value != nullptr) {
		do {
			std::cout << pair.first << " = '" << *pair.second << "'" << std::endl;
			pair = db.next();
		} while (pair.second != nullptr);
	}

	std::cout << "--------------------------------------------\n";
	std::cout << "ENTRIES: " << db.size() << "\n";
	std::cout << "CACHE HITS: " << db.getCacheHits() << "%\n";
}


void BosonAPITest::run() {

	insertData();
	db.printTreeState();
	eraseData();
	db.printTreeState();
	insertData();         // FIXME: RecordFileIO free list bug (free list is corrupted returning dublicates and corrupts tree)
	db.printTreeState();
	
	traverseEntries();

	eraseData();
	
}
