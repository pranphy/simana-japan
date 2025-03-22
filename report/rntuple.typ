#import "@local/tymer:0.1.0": *
#show: slides.with(
    authors: [Prakash Gautam\   #text(font:"Noto serif", fill: white.darken(71%))[〈 प्रकाश गौतम|  پرکاش گوتم 〉]],
    institute: Institute("University of Virginia",short:"UVa"),
    short-authors: "P. Gautam",
    //title: "Spectrometer, Beamline, Simulation and Timeline",
    title: "RNTuple status",
    location: "Analysis Meeting",
    //thm: (fg:black.darken(00%),bg:white.lighten(00%),ac: orange),
    //date: "2024-01-17",
    aspect: 16/9
)

#titlepage()


#set quote(block: true)
#slide(title: "Introduction")[
  #quote(attribution:[arxiv:2204.09043, RNTuple performence: Status and Outlook, 2 March 2023])[
    RNTuple is the new, experimental, backward-incompatible ROOT columnar I/O subsystem
    targeting high performance, reliability, and easy-to-use robust interfaces. Despite RNTuple still
    being under development, at this point it is feature-complete enough to carry out an evaluation
  ]
]

#slide(title: "Performance Comparisons [Jan 26, 2024]")[
  #show link: underline
  #grid(columns:(60%,40%),[
    #v(5em)
    - RNTuple is memory efficient compared to TTree.
    - Also show significant improvement in access times.
source: https://root.cern/blog/rntuple-update/ \

    - I couldn't find reliable references on write times.
  ],[
    #v(0.5em)
    //#image("./asset/image/rntuple_blog_size_mc.png",width:50%)
    #image("./asset/image/paste/20240427-121746-rntuple_blog_size_mc.png",width:62%)
    #image("./asset/image/rntuple_blog_adl.png",width:62%)
  ]
)
detailed: https://www.epj-conferences.org/articles/epjconf/pdf/2020/21/epjconf_chep2020_02030.pdf
//https://lhc-commissioning.web.cern.ch/schedule/LHC-long-term.htm
]


#slide(title: "Usage: Writing")[
  #show raw: set text(size: 0.7em)
   #grid(columns:(55%,45%),[
      ```cpp
      #include "ROOT/RNTuple.hxx"
      void rntuple_write(int N){
        auto model = RNTupleModel::Create();
        auto px = model->MakeField<float>("px");
        auto ntuple = RNTupleWriter::Recreate(
            std::move(model), "Event","rntuple.root");

        for(int i=0; i < N; ++i){
          *px = 1.0*i;
          ntuple->Fill();
        }
        // ntuple goes out of scope and is written
      }
```
    ],[
      ```cpp

      void tree_write(int N){
        auto f = new TFile("tree.root","RECREATE");
        auto T = new TTree("Event","desc");
        float px;
        T->Branch("px",&px,"px/F");

        for(int i=0; i < N; ++i){
          px = 1.0*i;
          T->Fill();
        }
        T->Write();
      }
      ```

    ]
  )
  - If it means anything : the produced `tree.root` is 1.2MB but `rntuple.root` is 85k on my machine.
  ```bash
  .rw-r--r--   85k  20 Apr 14:10  rntuple_1e6.root
  .rw-r--r--  747k  20 Apr 14:33  rntuple_10e6.root
  .rw-r--r--  1.2M  20 Apr 14:25  tree_1e6.root
  .rw-r--r--   13M  20 Apr 14:33  tree_10e6.root
  ```
]

#slide(title: "Usage: Reading")[
  #show raw: set text(size: 0.7em)
  - RDataFrame
      ```cpp
        ROOT::RDataFrame df = ROOT::RDF::Experimental::FromRNTuple("Test", "rntuple.root");
        auto hist = df.Histo1D("px");
        hist->Draw();

        auto h2 = df.Filter("px < 100").Histo1D("px");
        h2->Draw();
       ```
      - My Experience with RDataFrame is that it is extremely fast even on `TTree`
   

  - RNTupleReader
  ```cpp
   auto model = RNTupleModel::Create();
   std::shared_ptr<float> px = model->MakeField<float>("px");
   auto ntuple = RNTupleReader::Open(std::move(model),"Test","rntuple.root");
   TH1F h("pxhist", "Hist of px", 100, 0, 100);
 
   for (auto entryId : *ntuple) {
      ntuple->LoadEntry(entryId);
      h.Fill(*fldAge);
   }
  ```
  

]


#slide(title: "Updates ")[
  #quote(attribution:[Axel Naumann, Root Forum April 2023])[
   We are developing and releasing different parts at different pace, due to the need to focus our efforts. RDataFrame was one of the first elements to use the ROOT 7 style of interfaces; REve (2023), the web-based graphics system (2023), and RNtuple (2024) are on their way to production (respective release years)...
  ]

  #quote(attribution:[arxiv:2204.09043, RNTuple performence: Status and Outlook, 2 March 2023])[
    RNTuple is expected to become production grade in 2024.
  ]

]

#slide(title: "Readiness")[
  #quote(attribution:[Danilo (ROOT Developer), Root Forum April 2024])[
    #text(0.5em)[
    Thanks for your interest in RNTuple, which is the currently the flagship development project of ROOT. If this can be helpful, I would like to invite you to write to me privately, so that we can arrange a meeting about your project and its needs at your convenience.

    The answer to your question depends of course on the nature of the project you are starting.

    At the moment, in collaboration with LHC experiments and Dune, as well as several power users, we are working to make RNTuple functional in offline and online data processing environments as well as high performance data analysis setups. During 2024, we will fix the on-disk file format. API changes will have to be expected also after 2024: the precise timeline for those will be communicated in due time.

    In terms of releases for 2024 and H1 2025, the timeline is the following:

        6.32.00: End of May 2024 - 2025 Data taking release for LHC experiments\
        6.33.00: November 2024 - Development Release, not for production.\
        6.34.00: May 2025 (tbc according to the LHC schedule)\

    Hopefully that can be of help in planning which one to pick for your project.

    Best,
    Danilo
    ]
  ]
]




#slide(title: "Summary")[
  - RNTuple is significantly faster to read.
  - Depending upon compression it has significant storage improvement.
  - Currently in experimental namespace with API still possible to change.
  - Likely ready within 2025.
]
