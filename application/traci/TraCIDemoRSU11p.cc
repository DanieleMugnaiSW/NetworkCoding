//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "veins/modules/application/traci/TraCIDemoRSU11p.h"
#include <algorithm>
#include <iostream>
#include <math.h>
using namespace std;

Define_Module(TraCIDemoRSU11p);

bool compareID(WaveShortMessage* a, WaveShortMessage* b) {
    return (a->getId() < b->getId());
}
bool compare(WaveShortMessage::Key a, WaveShortMessage::Key b) {
    return (a.id < b.id);

}

void TraCIDemoRSU11p::onWSA(WaveServiceAdvertisment* wsa) {
//if this RSU receives a WSA for service 42, it will tune to the chan
    if (wsa->getPsid() == 42) {
        mac->changeServiceChannel(wsa->getTargetChannel());
    }
}

void TraCIDemoRSU11p::onWSM(WaveShortMessage* wsm) {
    int prob = std::round(uniform(0, 10000));
    if (prob <= 9000) {

        if (myId != 0) {
            if (myId == 3) {
                row++;

                if (store.size() == FLOW) {
                    simtime_t now = simTime();
                    std::cout << "LAST RECEIVER FULL AT " << now << std::endl;
                    this->endSimulation();
                }
            }

            bool found = false;
            for (int i = 0; i < store.size(); i++) {
                if (store[i]->getId() == wsm->getId()) {
                    found = true;
                }
            }
            if (!found) {
                store.push_back(wsm);
                //std::cout<<"RSU  "<<this->myId<<" STORE WSM   "<<wsm->getId()<<std::endl;
                //combine();

            }
            if (myId != 3) {
                //std::cout<<"RSU  "<<this->myId<<" INVIA WSM   "<<wsm->getId()<<std::endl;
                wsm->setSenderAddress(myId);
                sendDown(wsm->dup());
            }
            //sendDown(wsm->dup());

        }

        /*      if (this->myId != 0) {
         if (this->myId != 4 ) {
         count++;
         wsm->setSenderAddress(myId);
         sendDown(wsm->dup());
         } else {
         for (int j = 0; j < wsm->bufferkey.size(); j++) {
         int col = wsm->bufferkey[j].id;
         //std::cout << "id key :" << col << std::endl;
         int exp = std::round(uniform(0, 7));
         this->m(row, col) = pow(2, exp);
         }
         row++;
         int i = 0;

         while (inverti == false && i < m.rows()) {
         if (m.block(i, 0, FLOW, FLOW).determinant() != 0) { // if det != 0 i can invert!
         MatrixXd inverse = m.block(i, 0, FLOW, FLOW).inverse();
         result = (m.block(i, 0, FLOW, FLOW)) * inverse;
         inverti = true;
         } else
         i++;
         }
         if (inverti) {

         this->endSimulation();

         }

         }
         */
        /* if (wsm->bufferkey.size() <= 1) {
         std::cout << "SINGLETON" << std::endl;
         count++;
         WaveShortMessage* decoded = new WaveShortMessage();
         decoded->setId(wsm->bufferkey[0].id);

         bool found = false;
         for (int k = 0; k < store.size(); k++) {
         if (store[k]->getId() == decoded->getId())
         found = true;

         }
         if (!found) {
         store.push_back(decoded);
         }

         } else {
         //std::cout << "NOT SINGLETON--->DECAPSULATION " << std::endl;
         for (int j = 0; j < wsm->bufferkey.size(); j++) {
         bool foundDup = false;
         for (int i = 0; i < tmp_store.size(); i++) {
         if (tmp_store[i].id == wsm->dup()->bufferkey[j].id)
         foundDup = true;

         }
         if (foundDup == false) {
         this->tmp_store.push_back(wsm->dup()->bufferkey[j]);
         //std::cout << "INSERISCO  " << wsm->dup()->bufferkey[j].id<< std::endl;
         }

         std::sort(tmp_store.begin(), tmp_store.end(), compare);
         int col = wsm->bufferkey[j].id;
         //std::cout << "id key :" << col << std::endl;
         int exp = std::round(uniform(0, 7));
         this->m(row, col) = pow(2, exp);

         }
         row++;
         if (myId != 2) {
         for (int k = 0; k < 4 * FLOW; k++) {
         WaveShortMessage* wsmcombined = new WaveShortMessage(); //message combined
         int toCombine = std::round(uniform(1, FLOW)); //num of message to be combined
         //int toCombine = 3;
         //std::cout << "NUMBER OF MSG TO COMBINE:  " << toCombine<< std::endl;
         for (int i = 0; i < toCombine; i++) {

         int index = (int) std::round(
         uniform(0, this->tmp_store.size() - 1));

         //std::cout << "INDEX CHOOSE:  " << index << "  "<< std::endl;

         wsmcombined->bufferkey.push_back(tmp_store[index]);

         }
         populateWSM(wsmcombined);
         wsmcombined->setName("data");

         sendDown(wsmcombined->dup());
         }
         } else {
         int i = 0;

         while (inverti == false && i < m.rows()) {
         if (m.block(i, 0, FLOW, FLOW).determinant() != 0) { // if det != 0 i can invert!
         MatrixXd inverse =
         m.block(i, 0, FLOW, FLOW).inverse();
         result = (m.block(i, 0, FLOW, FLOW)) * inverse;
         inverti = true;
         } else
         i++;
         }
         if (inverti) {

         this->endSimulation();

         }
         }

         }

         }

         }*/
    } else {
        delete (wsm);
        cout << "PACKET LOST" << endl;
        count++;
    }
}
void TraCIDemoRSU11p::finish() {
    /*
     std::cout << "--RSU  " << this->myId << "  --" << std::endl;
     std::cout << this->row << std::endl;

     std::cout << this->count << endl;
     cout << count + row << endl;
     */
    /*
     std::sort(store.begin(), store.end(), compareID); //sort store to see missing message
     for (int i = 0; i < store.size(); i++) {
     std::cout << store[i]->getId() << "  ";
     }
     std::cout << "--MY STORE  " << this->myId << "  --" << std::endl;
     for (int i = 0; i < tmp_store.size(); i++) {
     std::cout << tmp_store[i].id << " ";
     }
     */
    std::cout << endl;
    if (myId == 3) {
        /* std::cout << this->result << std::endl;
         std::cout << endl;

         std::cout << this->m << std::endl;
         std::cout << endl;

         cout << count + row << endl;
         */
        std::sort(store.begin(), store.end(), compareID); //sort store to see missing message
        for (int i = 0; i < store.size(); i++) {
            std::cout << store[i]->getId() << "  ";
        }
        std::cout << endl;
        cout << count + row << endl;

}
}

