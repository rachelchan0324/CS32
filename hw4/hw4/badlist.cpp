void removeBad(list<Movie*>& li) {
    list<Movie*>::iterator it = li.begin();
    while(it != li.end()){
        if((*it)->rating() < 50){
            delete *it; // delete movie pointed to by Movie*
            it = li.erase(it); // delete Movie*
        }
        else
            it++;
    }
}
