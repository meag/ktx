// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

void map_ztndm6() {
	N(701, 755, 24);
	N(702, 463, 24);
	N(869, 624, -152);
	N(697, 1033, 24);
	N(716, 167, 24);
	N(581, 1112, 24);
	N(378, 893, 40);
	N(373, 740, 88);
	N(74, 602, 88);
	N(-72, 295, 88);
	N(-255, 473, 40);
	N(-661, 407, 24);
	N(-625, 26, 24);
	N(-407, -36, 24);
	N(-432, 84, -136);
	N(-189, 135, 24);
	N(4, 113, 24);
	N(593, 80, 24);
	N(206, -120, 24);
	N(270, -190, -40);
	N(421, -23, -136);
	N(364, 108, -168);
	N(410, 288, -168);
	N(245, 423, -168);
	N(119, 656, -168);
	N(5, 743, -136);
	N(-191, 893, -136);
	N(203, 1086, 24);
	N(-657, -125, 24);
	N(-674, 374, -616);
	N(416, -160, -88);
	LSQ();
	G1(m1);
	Z1(m1);
	m1->fb.paths[0].next_marker = m63;
	m1->fb.paths[1].next_marker = m23;
	m1->fb.paths[2].next_marker = m16;
	G2(m2);
	Z1(m2);
	m2->fb.paths[0].next_marker = m62;
	m2->fb.paths[1].next_marker = m1;
	m2->fb.paths[2].next_marker = m61;
	m2->fb.paths[0].flags = JUMP_LEDGE;
	m2->fb.paths[1].flags = JUMP_LEDGE;
	m2->fb.paths[2].flags = JUMP_LEDGE;
	Z1(m3);
	m3->fb.paths[0].next_marker = m63;
	Z5(m4);
	m4->fb.paths[0].next_marker = m5;
	m4->fb.paths[1].next_marker = m89;
	Z5(m5);
	m5->fb.paths[0].next_marker = m12;
	m5->fb.paths[1].next_marker = m4;
	Z7(m6);
	m6->fb.paths[0].next_marker = m7;
	m6->fb.paths[1].next_marker = m9;
	m7->s.v.view_ofs[2] = -2;
	Z7(m7);
	m7->fb.paths[0].next_marker = m22;
	m7->fb.paths[1].next_marker = m71;
	m7->fb.paths[2].next_marker = m6;
	Z4(m8);
	m8->fb.paths[0].next_marker = m7;
	m9->s.v.view_ofs[2] = -2;
	Z4(m9);
	m9->fb.paths[0].next_marker = m83;
	m9->fb.paths[1].next_marker = m8;
	G3(m10);
	Z5(m10);
	m10->fb.paths[0].next_marker = m73;
	m10->fb.paths[1].next_marker = m74;
	m10->fb.paths[2].next_marker = m75;
	m10->fb.paths[0].flags = JUMP_LEDGE;
	m10->fb.paths[1].flags = JUMP_LEDGE;
	m10->fb.paths[2].flags = JUMP_LEDGE;
	Z5(m11);
	m11->fb.paths[0].next_marker = m75;
	m11->fb.paths[1].next_marker = m13;
	Z5(m12);
	m12->fb.paths[0].next_marker = m13;
	m12->fb.paths[1].next_marker = m5;
	Z5(m13);
	m13->fb.paths[0].next_marker = m11;
	m13->fb.paths[1].next_marker = m12;
	G14(m16);
	Z1(m16);
	m16->fb.paths[0].next_marker = m1;
	m16->fb.paths[1].next_marker = m23;
	G4(m17);
	Z2(m17);
	m17->fb.paths[0].next_marker = m41;
	m17->fb.paths[1].next_marker = m31;
	m17->fb.paths[2].next_marker = m69;
	G5(m18);
	Z4(m18);
	m18->fb.paths[0].next_marker = m20;
	m18->fb.paths[1].next_marker = m19;
	m18->fb.paths[2].next_marker = m28;
	Z4(m19);
	m19->fb.paths[0].next_marker = m20;
	m19->fb.paths[1].next_marker = m18;
	m19->fb.paths[2].next_marker = m83;
	m19->fb.paths[3].next_marker = m84;
	Z4(m20);
	m20->fb.paths[0].next_marker = m82;
	m20->fb.paths[1].next_marker = m19;
	m20->fb.paths[2].next_marker = m18;
	G6(m21);
	Z2(m21);
	m21->fb.paths[0].next_marker = m68;
	m21->fb.paths[1].next_marker = m29;
	m21->fb.paths[2].next_marker = m41;
	m21->fb.paths[3].next_marker = m69;
	G7(m22);
	Z7(m22);
	m22->fb.paths[0].next_marker = m71;
	m22->fb.paths[1].next_marker = m25;
	m22->fb.paths[2].next_marker = m24;
	m22->fb.paths[3].next_marker = m7;
	m22->fb.paths[4].next_marker = m36;
	G14(m23);
	Z1(m23);
	m23->fb.paths[0].next_marker = m1;
	m23->fb.paths[1].next_marker = m16;
	G8(m24);
	Z7(m24);
	m24->fb.paths[0].next_marker = m25;
	m24->fb.paths[1].next_marker = m22;
	m24->fb.paths[2].next_marker = m36;
	m24->fb.paths[3].next_marker = m72;
	G8(m25);
	Z7(m25);
	m25->fb.paths[0].next_marker = m71;
	m25->fb.paths[1].next_marker = m22;
	m25->fb.paths[2].next_marker = m24;
	G9(m26);
	Z4(m26);
	m26->fb.paths[0].next_marker = m28;
	m26->fb.paths[1].next_marker = m27;
	G9(m27);
	Z4(m27);
	m27->fb.paths[0].next_marker = m28;
	m27->fb.paths[1].next_marker = m26;
	G10(m28);
	Z4(m28);
	m28->fb.paths[0].next_marker = m18;
	m28->fb.paths[1].next_marker = m84;
	m28->fb.paths[2].next_marker = m85;
	m28->fb.paths[3].next_marker = m27;
	m28->fb.paths[4].next_marker = m26;
	G11(m29);
	Z2(m29);
	m29->fb.paths[0].next_marker = m21;
	m29->fb.paths[1].next_marker = m68;
	G12(m30);
	Z6(m30);
	m30->fb.paths[0].next_marker = m77;
	m30->fb.paths[1].next_marker = m46;
	m30->fb.paths[2].next_marker = m79;
	G4(m31);
	Z2(m31);
	m31->fb.paths[0].next_marker = m17;
	m31->fb.paths[1].next_marker = m39;
	m31->fb.paths[2].next_marker = m69;
	G13(m32);
	Z7(m32);
	m32->fb.paths[0].next_marker = m70;
	m32->fb.paths[1].next_marker = m71;
	Z3(m33);
	m33->fb.paths[0].next_marker = m87;
	m33->fb.paths[1].next_marker = m35;
	m33->fb.paths[2].next_marker = m34;
	m33->fb.paths[3].next_marker = m43;
	m33->fb.paths[4].next_marker = m45;
	Z3(m34);
	m34->fb.paths[0].next_marker = m35;
	m34->fb.paths[1].next_marker = m43;
	m34->fb.paths[2].next_marker = m33;
	Z3(m35);
	m35->fb.paths[0].next_marker = m87;
	m35->fb.paths[1].next_marker = m34;
	m35->fb.paths[2].next_marker = m33;
	Z7(m36);
	m36->fb.paths[0].next_marker = m22;
	m36->fb.paths[1].next_marker = m24;
	m36->fb.paths[2].next_marker = m72;
	m37->s.v.view_ofs[2] = 17;
	Z6(m37);
	m37->fb.paths[0].next_marker = m46;
	m37->fb.paths[1].next_marker = m78;
	Z5(m38);
	m38->fb.paths[0].next_marker = m75;
	m38->fb.paths[1].next_marker = m59;
	m39->s.v.view_ofs[2] = 17;
	Z2(m39);
	m39->fb.paths[0].next_marker = m31;
	m39->fb.paths[1].next_marker = m47;
	m39->fb.paths[2].next_marker = m69;
	m40->s.v.view_ofs[2] = -10;
	Z3(m40);
	m40->fb.paths[0].next_marker = m86;
	m40->fb.paths[1].next_marker = m87;
	G4(m41);
	Z2(m41);
	m41->fb.paths[0].next_marker = m17;
	m41->fb.paths[1].next_marker = m21;
	m41->fb.paths[2].next_marker = m68;
	Z3(m42);
	m42->fb.paths[0].next_marker = m45;
	m42->fb.paths[1].next_marker = m88;
	m43->s.v.view_ofs[2] = 17;
	Z3(m43);
	m43->fb.paths[0].next_marker = m34;
	m43->fb.paths[1].next_marker = m33;
	m43->fb.paths[2].next_marker = m45;
	G15(m44);
	Z8(m44);
	Z3(m45);
	m45->fb.paths[0].next_marker = m33;
	m45->fb.paths[1].next_marker = m43;
	m45->fb.paths[2].next_marker = m42;
	G16(m46);
	Z6(m46);
	m46->fb.paths[0].next_marker = m30;
	m46->fb.paths[1].next_marker = m77;
	m46->fb.paths[2].next_marker = m37;
	m46->fb.paths[3].next_marker = m79;
	Z2(m47);
	m47->fb.paths[0].next_marker = m39;
	m47->fb.paths[1].next_marker = m48;
	m47->fb.paths[2].next_marker = m69;
	Z2(m48);
	m48->fb.paths[0].next_marker = m47;
	m48->fb.paths[1].next_marker = m69;
	m48->fb.paths[2].next_marker = m70;
	Z5(m49);
	m49->fb.paths[0].next_marker = m74;
	m49->fb.paths[1].next_marker = m76;
	Z9(m50);
	m50->fb.paths[0].next_marker = m58;
	m51->s.v.view_ofs[2] = -10;
	Z9(m51);
	m51->fb.paths[0].next_marker = m90;
	G17(m52);
	Z9(m52);
	m52->fb.paths[0].next_marker = m90;
	G17(m53);
	Z9(m53);
	m53->fb.paths[0].next_marker = m90;
	G17(m54);
	Z9(m54);
	m54->fb.paths[0].next_marker = m90;
	G17(m55);
	Z9(m55);
	m55->fb.paths[0].next_marker = m90;
	G17(m56);
	Z9(m56);
	m56->fb.paths[0].next_marker = m90;
	m57->s.v.view_ofs[2] = -2;
	Z7(m57);
	m58->s.v.view_ofs[2] = -10;
	Z2(m58);
	m58->fb.paths[0].next_marker = m66;
	m58->fb.paths[1].next_marker = m67;
	m58->fb.paths[2].next_marker = m88;
	m59->s.v.view_ofs[2] = -10;
	Z5(m59);
	m59->fb.paths[0].next_marker = m38;
	m60->s.v.view_ofs[2] = 33;
	Z1(m60);
	Z1(m61);
	m61->fb.paths[0].next_marker = m1;
	m61->fb.paths[1].next_marker = m2;
	m61->fb.paths[2].next_marker = m62;
	m61->fb.paths[3].next_marker = m64;
	m61->fb.paths[0].flags = JUMP_LEDGE;
	m61->fb.paths[1].flags = JUMP_LEDGE;
	m61->fb.paths[2].flags = JUMP_LEDGE;
	Z1(m62);
	m62->fb.paths[0].next_marker = m65;
	m62->fb.paths[1].next_marker = m2;
	m62->fb.paths[2].next_marker = m1;
	m62->fb.paths[3].next_marker = m61;
	m62->fb.paths[1].flags = JUMP_LEDGE;
	m62->fb.paths[2].flags = JUMP_LEDGE;
	m62->fb.paths[3].flags = JUMP_LEDGE;
	Z1(m63);
	m63->fb.paths[0].next_marker = m1;
	m63->fb.paths[1].next_marker = m3;
	Z1(m64);
	m64->fb.paths[0].next_marker = m61;
	m64->fb.paths[1].next_marker = m66;
	Z6(m65);
	m65->fb.paths[0].next_marker = m62;
	m65->fb.paths[1].next_marker = m78;
	Z2(m66);
	m66->fb.paths[0].next_marker = m64;
	m66->fb.paths[1].next_marker = m58;
	Z2(m67);
	m67->fb.paths[0].next_marker = m58;
	m67->fb.paths[1].next_marker = m68;
	Z2(m68);
	m68->fb.paths[0].next_marker = m67;
	m68->fb.paths[1].next_marker = m21;
	m68->fb.paths[2].next_marker = m29;
	m68->fb.paths[3].next_marker = m41;
	Z2(m69);
	m69->fb.paths[0].next_marker = m17;
	m69->fb.paths[1].next_marker = m31;
	m69->fb.paths[2].next_marker = m39;
	m69->fb.paths[3].next_marker = m47;
	m69->fb.paths[4].next_marker = m48;
	m69->fb.paths[5].next_marker = m21;
	Z7(m70);
	m70->fb.paths[0].next_marker = m48;
	m70->fb.paths[1].next_marker = m69;
	m70->fb.paths[2].next_marker = m32;
	Z7(m71);
	m71->fb.paths[0].next_marker = m32;
	m71->fb.paths[1].next_marker = m25;
	m71->fb.paths[2].next_marker = m22;
	m71->fb.paths[3].next_marker = m7;
	Z7(m72);
	m72->fb.paths[0].next_marker = m36;
	m72->fb.paths[1].next_marker = m24;
	m72->fb.paths[2].next_marker = m73;
	Z5(m73);
	m73->fb.paths[0].next_marker = m74;
	m73->fb.paths[1].next_marker = m72;
	m73->fb.paths[2].next_marker = m10;
	m73->fb.paths[3].next_marker = m89;
	m73->fb.paths[4].next_marker = m75;
	m73->fb.paths[4].flags = JUMP_LEDGE;
	Z5(m74);
	m74->fb.paths[0].next_marker = m73;
	m74->fb.paths[1].next_marker = m10;
	m74->fb.paths[2].next_marker = m49;
	m74->fb.paths[3].next_marker = m89;
	m74->fb.paths[1].flags = JUMP_LEDGE;
	Z5(m75);
	m75->fb.paths[0].next_marker = m11;
	m75->fb.paths[1].next_marker = m38;
	Z6(m76);
	m76->fb.paths[0].next_marker = m49;
	m76->fb.paths[1].next_marker = m77;
	Z6(m77);
	m77->fb.paths[0].next_marker = m76;
	m77->fb.paths[1].next_marker = m30;
	m77->fb.paths[2].next_marker = m46;
	Z6(m78);
	m78->fb.paths[0].next_marker = m65;
	m78->fb.paths[1].next_marker = m37;
	Z6(m79);
	m79->fb.paths[0].next_marker = m30;
	m79->fb.paths[1].next_marker = m46;
	m79->fb.paths[2].next_marker = m80;
	Z6(m80);
	m80->fb.paths[0].next_marker = m79;
	m80->fb.paths[1].next_marker = m91;
	Z6(m81);
	m81->fb.paths[0].next_marker = m91;
	m81->fb.paths[1].next_marker = m82;
	Z6(m82);
	m82->fb.paths[0].next_marker = m81;
	m82->fb.paths[1].next_marker = m20;
	m82->fb.paths[2].next_marker = m83;
	Z4(m83);
	m83->fb.paths[0].next_marker = m82;
	m83->fb.paths[1].next_marker = m19;
	m83->fb.paths[3].next_marker = m9;
	m83->fb.paths[4].next_marker = m84;
	Z4(m84);
	m84->fb.paths[0].next_marker = m19;
	m84->fb.paths[1].next_marker = m83;
	m84->fb.paths[2].next_marker = m28;
	m84->fb.paths[3].next_marker = m85;
	Z4(m85);
	m85->fb.paths[0].next_marker = m28;
	m85->fb.paths[1].next_marker = m84;
	m85->fb.paths[2].next_marker = m86;
	Z3(m86);
	m86->fb.paths[0].next_marker = m85;
	m86->fb.paths[1].next_marker = m40;
	Z3(m87);
	m87->fb.paths[0].next_marker = m40;
	m87->fb.paths[1].next_marker = m35;
	m87->fb.paths[2].next_marker = m33;
	Z3(m88);
	m88->fb.paths[0].next_marker = m42;
	m88->fb.paths[1].next_marker = m58;
	Z5(m89);
	m89->fb.paths[0].next_marker = m4;
	m89->fb.paths[1].next_marker = m74;
	m89->fb.paths[2].next_marker = m73;
	Z9(m90);
	m90->fb.paths[0].next_marker = m52;
	m90->fb.paths[1].next_marker = m54;
	m90->fb.paths[2].next_marker = m53;
	m90->fb.paths[3].next_marker = m55;
	m90->fb.paths[4].next_marker = m56;
	m90->fb.paths[5].next_marker = m51;
	Z6(m91);
	m91->fb.paths[0].next_marker = m81;
	m91->fb.paths[1].next_marker = m80;
}

