%% train & test & evaluation

Y_train = yqtrain(:,11);
X_train = yqtrain(:,1:10);
X_test =  yqtest(:,1:10);
[ Y_test, forest_model ] = random_test( Y_train, X_train, X_test);



