// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

gedict_t* AddToList(gedict_t* first_in_list, gedict_t* ent) {
	ent->fb.next = first_in_list;
	if (first_in_list) {
		ent->fb.previous = first_in_list->fb.previous;
		if (first_in_list->fb.previous) {
			first_in_list->fb.previous->fb.next = ent;
		}
		first_in_list->fb.previous = ent;
	}
	return ent;
}

gedict_t* RemoveFromList(gedict_t* first_in_list, gedict_t* ent) {
	if (first_in_list == ent) {
		first_in_list = first_in_list->fb.next;
	}
	if (ent->fb.previous) {
		ent->fb.previous->fb.next = ent->fb.next;
	}
	if (ent->fb.next) {
		ent->fb.next->fb.previous = ent->fb.previous;
	}
	return first_in_list;
}

gedict_t* EntityAt(gedict_t* start_entity, float index_) {
	while (index_ > 0) {
		start_entity = nextent(start_entity);
		index_ = index_ - 1;
	}
	return start_entity;
}

