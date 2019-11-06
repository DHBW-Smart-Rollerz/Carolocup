%mit diesem Skript wird die optimale Reglerstruktur k1 und k2 berechnet
%zur vereinfachung wird v konstant auf die maximale kurvengesch. von
%2.63m/s bei 1m kurvenradius gesetzt
%als Eingangsgr��e dient die Kurvenkr�mmung c0 die aus dem Kamerasystem
%kommt. Evtl besser: kamera liefert soll Kurswinkel delta_psi -> c0 =
%f(delta_psi)
systemMatrix_A = [0,2.63;0,0];%maximale Kurvengeschwindigkeit von 2.63m/s
ausgangsVektor_cT = [0,2/0.5];%wegen vorder und hinterache gilt: Kurswinkel ist 0.5 des vorderachseinschlags (ungewichtetes mittel aus vorder und hinterachse)
eingangsVektor_b = [0;-2.63];%delta_psi' = psi' - c0*v (letzteres ist der eingang, ersterees wird zus�tlich in simulink addiert)
system = ss(systemMatrix_A, eingangsVektor_b, ausgangsVektor_cT, 0);
K = lqr(system, [3*0.5,0;0,0.1], 1)%tbd was sind Q, N, R? es handelt sich um Gewichtungsmatrixen in Diagonalform Q:Gewichtung des Zustandsvektors, R: Gewichtung der Eingangsvariable, N:Gewichtung beider gr��en???
