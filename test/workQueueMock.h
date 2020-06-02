//
// Created by chris on 5/29/20.
//

#ifndef MATRIXOPERATIONS_WORKQUEUEMOCK_H
#define MATRIXOPERATIONS_WORKQUEUEMOCK_H

#include "gmock/gmock.h"
#include "../../src/libtp/workQueue.h"



template<class T>
class workQueueMock : public tp::workQueue<T> {

public:
    MOCK_METHOD(void, addWork, (std::function<T()> fucntion), (override));
    MOCK_METHOD(tp::task_s<T>*, dequeueWork, (), (ovveride));



};


#endif //MATRIXOPERATIONS_WORKQUEUEMOCK_H
