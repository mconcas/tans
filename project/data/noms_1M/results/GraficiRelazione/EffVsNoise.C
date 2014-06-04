{
//=========Macro generated from canvas: Canvas_1_n3/Canvas_1
//=========  (Wed Jun  4 14:24:34 2014) by ROOT version5.34/18
   TCanvas *Canvas_1_n3 = new TCanvas("Canvas_1_n3", "Canvas_1",190,115,911,660);
   Canvas_1_n3->ToggleEventStatus();
   Canvas_1_n3->ToggleToolTips();
   Canvas_1_n3->Range(-3.30192,0.8774957,38.2347,1.021494);
   Canvas_1_n3->SetFillColor(0);
   Canvas_1_n3->SetBorderMode(0);
   Canvas_1_n3->SetBorderSize(2);
   Canvas_1_n3->SetFrameBorderMode(0);
   Canvas_1_n3->SetFrameBorderMode(0);
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(6);
   grae->SetName("EfficiencyVsNoise");
   grae->SetTitle("Efficiency vs Noise");
   grae->SetFillStyle(3009);
   grae->SetLineColor(4);
   grae->SetLineStyle(3);
   grae->SetMarkerColor(2);
   grae->SetPoint(0,2.583333,0.997232);
   grae->SetPointError(0,2.583333,2.583333,5.354243e-05,5.25385e-05);
   grae->SetPoint(1,7.75,0.957555);
   grae->SetPointError(1,2.583333,2.583333,0.0002025611,0.0002016437);
   grae->SetPoint(2,12.91667,0.948173);
   grae->SetPointError(2,2.583333,2.583333,0.000222627,0.0002217286);
   grae->SetPoint(3,18.08333,0.935007);
   grae->SetPointError(3,2.583333,2.583333,0.0002474495,0.0002465778);
   grae->SetPoint(4,23.25,0.918672);
   grae->SetPointError(4,2.583333,2.583333,0.0002742577,0.0002734188);
   grae->SetPoint(5,28.41667,0.901752);
   grae->SetPointError(5,2.583333,2.583333,0.0002985521,0.0002977473);
   
   TH1F *Graph_Graph_EfficiencyVsNoise14 = new TH1F("Graph_Graph_EfficiencyVsNoise14","Efficiency vs Noise",100,0.775,34.01389);
   Graph_Graph_EfficiencyVsNoise14->SetMinimum(0.8918703);
   Graph_Graph_EfficiencyVsNoise14->SetMaximum(1.006868);
   Graph_Graph_EfficiencyVsNoise14->SetDirectory(0);
   Graph_Graph_EfficiencyVsNoise14->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000099");
   Graph_Graph_EfficiencyVsNoise14->SetLineColor(ci);
   Graph_Graph_EfficiencyVsNoise14->GetXaxis()->SetTitle("Noise level");
   Graph_Graph_EfficiencyVsNoise14->GetXaxis()->CenterTitle(true);
   Graph_Graph_EfficiencyVsNoise14->GetXaxis()->SetNdivisions(10510);
   Graph_Graph_EfficiencyVsNoise14->GetXaxis()->SetLabelFont(42);
   Graph_Graph_EfficiencyVsNoise14->GetXaxis()->SetLabelSize(0.03);
   Graph_Graph_EfficiencyVsNoise14->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph_EfficiencyVsNoise14->GetXaxis()->SetTitleFont(52);
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->SetTitle("Efficiency (Evts_rec/Evts_gen)");
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->CenterTitle(true);
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->SetLabelFont(42);
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->SetLabelSize(0.03);
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->SetTickLength(0.02);
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->SetTitleOffset(1.2);
   Graph_Graph_EfficiencyVsNoise14->GetYaxis()->SetTitleFont(52);
   Graph_Graph_EfficiencyVsNoise14->GetZaxis()->SetLabelFont(42);
   Graph_Graph_EfficiencyVsNoise14->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph_EfficiencyVsNoise14->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph_EfficiencyVsNoise14->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_Graph_EfficiencyVsNoise14);
   
   grae->Draw("AP");
   
   TPaveText *pt = new TPaveText(0.3271824,0.9277496,0.6728176,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("Efficiency vs Noise");
   pt->Draw();
   Canvas_1_n3->Modified();
   Canvas_1_n3->cd();
   Canvas_1_n3->SetSelected(Canvas_1_n3);
   Canvas_1_n3->ToggleToolBar();
}
