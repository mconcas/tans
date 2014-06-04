{
//=========Macro generated from canvas: Canvas_1_n3/Canvas_1
//=========  (Wed Jun  4 14:25:45 2014) by ROOT version5.34/18
   TCanvas *Canvas_1_n3 = new TCanvas("Canvas_1_n3", "Canvas_1",212,115,889,660);
   Canvas_1_n3->ToggleEventStatus();
   Canvas_1_n3->ToggleToolTips();
   Canvas_1_n3->Range(-12.29868,0.9259477,12.38419,0.9524557);
   Canvas_1_n3->SetFillColor(0);
   Canvas_1_n3->SetBorderMode(0);
   Canvas_1_n3->SetBorderSize(2);
   Canvas_1_n3->SetFrameBorderMode(0);
   Canvas_1_n3->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(9);
   grae->SetName("EfficiencyVsZ");
   grae->SetTitle("Efficiency vs Z coord");

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   grae->SetLineColor(ci);
   grae->SetPoint(0,-7.315556,0.9307815);
   grae->SetPointError(0,0.9144444,0.9144444,0.0004236072,0.0004212288);
   grae->SetPoint(1,-5.486667,0.938772);
   grae->SetPointError(1,0.9144444,0.9144444,0.0003251236,0.0003235222);
   grae->SetPoint(2,-3.657778,0.9439091);
   grae->SetPointError(2,0.9144444,0.9144444,0.000269237,0.00026803);
   grae->SetPoint(3,-1.828889,0.9475851);
   grae->SetPointError(3,0.9144444,0.9144444,0.000238298,0.0002372813);
   grae->SetPoint(4,1.110223e-16,0.9477689);
   grae->SetPointError(4,0.9144444,0.9144444,0.0002309975,0.0002300385);
   grae->SetPoint(5,1.828889,0.9465447);
   grae->SetPointError(5,0.9144444,0.9144444,0.00024076,0.0002397436);
   grae->SetPoint(6,3.657778,0.9440565);
   grae->SetPointError(6,0.9144444,0.9144444,0.0002687695,0.0002675633);
   grae->SetPoint(7,5.486667,0.9400026);
   grae->SetPointError(7,0.9144444,0.9144444,0.0003222398,0.0003206322);
   grae->SetPoint(8,7.315556,0.9312138);
   grae->SetPointError(8,0.9144444,0.9144444,0.0004224733,0.0004200916);
   
   TH1F *Graph_Graph_EfficiencyVsZ25 = new TH1F("Graph_Graph_EfficiencyVsZ25","Efficiency vs Z coord",100,-9.876,9.876);
   Graph_Graph_EfficiencyVsZ25->SetMinimum(0.9285938);
   Graph_Graph_EfficiencyVsZ25->SetMaximum(0.9497631);
   Graph_Graph_EfficiencyVsZ25->SetDirectory(0);
   Graph_Graph_EfficiencyVsZ25->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph_EfficiencyVsZ25->SetLineColor(ci);
   Graph_Graph_EfficiencyVsZ25->GetXaxis()->SetTitle("Z_coord (cm)");
   Graph_Graph_EfficiencyVsZ25->GetXaxis()->CenterTitle(true);
   Graph_Graph_EfficiencyVsZ25->GetXaxis()->SetNdivisions(50510);
   Graph_Graph_EfficiencyVsZ25->GetXaxis()->SetLabelFont(42);
   Graph_Graph_EfficiencyVsZ25->GetXaxis()->SetLabelSize(0.03);
   Graph_Graph_EfficiencyVsZ25->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph_EfficiencyVsZ25->GetXaxis()->SetTitleFont(52);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetTitle("Efficiency (Evts_rec/Evts_gen)");
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->CenterTitle(true);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetNdivisions(30510);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetLabelFont(42);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetLabelSize(0.03);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetTickLength(0.02);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetTitleOffset(1.23);
   Graph_Graph_EfficiencyVsZ25->GetYaxis()->SetTitleFont(52);
   Graph_Graph_EfficiencyVsZ25->GetZaxis()->SetLabelFont(42);
   Graph_Graph_EfficiencyVsZ25->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph_EfficiencyVsZ25->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph_EfficiencyVsZ25->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph_EfficiencyVsZ25);
   
   grae->Draw("AP");
   
   TPaveText *pt = new TPaveText(0.2745763,0.9296029,0.7242938,0.9963899,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Efficiency vs Z coord");
   pt->Draw();
   Canvas_1_n3->Modified();
   Canvas_1_n3->cd();
   Canvas_1_n3->SetSelected(Canvas_1_n3);
   Canvas_1_n3->ToggleToolBar();
}
