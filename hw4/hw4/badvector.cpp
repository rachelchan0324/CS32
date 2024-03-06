void removeBad(vector<Movie*>& v){
    vector<Movie*>::iterator it = v.begin();
    while(it != v.end()){
        if((*it)->rating() < 50){
            delete *it; // delete movie pointed to by Movie*
            v.erase(it); // delete Movie*
        }
        else
            it++;
    }
}
