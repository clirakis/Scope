{
    gSystem->Load("libMatrix");
    TVectorD X(3), A(3);
    TMatrixD M(3,3), Q(3,3);

    X(0) = 1.0;
    X(1) = 2.0;
    X(2) = 3.0;

    X.Print();

    M(0,0) = 2.0;
    M(0,1) = 0.5;
    M(1,1) = 1.0;
    M(2,2) = 1.0;
    Q.Transpose(M);
    M.Print();
    Q.Print();
    A = M*X;
    A.Print();
}
