function [ Y_soft, model ] = random_test( Y_train, X_train, X_test )
%RANDOM_TEST Summary of this function goes here
%   Detailed explanation goes here

    
    opts= struct;
    opts.depth= 8;
    opts.numTrees= 500;
    opts.numSplits= 6;
%     opts.depth= 10;
%     opts.numTrees= 1000;
%     opts.numSplits= 8;
    opts.verbose= true;
    opts.classifierID= 2; % weak learners to use. Can be an array for mix of weak learners too

    tic
    model= forestTrain(X_train, Y_train, opts);
    toc
    tic
    [ Y_hard, Y_soft ] = forestTest(model, X_test);
    toc
end

