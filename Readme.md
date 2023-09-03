
# Study of decay $B^0(\bar{B^{0}})\to K^+K^-\pi^0$

😀😄🧑‍💻😺

basf2 version: `release-06-01-12`

## TO DO

<details><summary>
2023.8
</summary>

- [x] use git to manager my analysis files
- [x] add flavor tag
- [x] study the correlation between variables
</details>

<details><summary>
2023.9
</summary>

- [ ] give a brief report on charmless group
- [ ] generate more signal MC sample
- [ ] toy 2d fit
- [ ] fit to data and get branching fraction
- [ ] Amplitude analysis
</details>

## Update logs

Time | Description |
------- | ------- |
[2023](./notes/log/2023.md) |  The first month to log



## Selection criteria

### Preliminary selections

[The distribution of different variables](./notes/preliminary_selections.md)


- kaon selection
	- kaonID>0.2
- $\pi^0$ selection
	- gamma
		- clusterNHits > 1.5 and 0.2967 < clusterTheta < 2.6180
		- gamma in different range:
			- clusterReg=1: E>0.1 GeV
			- clusterReg=2: E>0.06 GeV
			- clusterReg=3: E>0.1 GeV
		- cluster shape: cluseterE9E21>0.9
	- $\pi^0$
		- the mass window used now: $0.111 \lt M_{\pi_{0}} \lt 0.153 \rm~GeV/c^2$, invariant mass between m_mean +- 3sigma.
        - $|\cos\theta_{helicity}|<0.95$
- $B^0$
  - vertex fit
  - delta E
  - Mbc

### Best $B^0$ candidate selection

We used two variables to select the best $B^0$ candidate: 
- $\pi^0$ mass constrained chi2 
- chiProb of $B^0$ vertex fit chi2

Selection strategy:
- Firstly, we select the $B^0$ candidate whose $\pi^0$ with lowest chi2. 
- If there are still multi $B^0$ candidate, we select the $B^0$ candidate with higher chiProb.

## Continuum suppression

MC sample: MC 15 run-indepent qqbar sample

We used FBDT to do continuum suppression, which is introduced in the [basf2 website](https://software.belle2.org/development/sphinx/online_book/basf2/cs.html).

We used the [MVA package](https://software.belle2.org/development/sphinx/mva/doc/index-01-mva.html) in basf2 to mix signal and background sample, and evaluate the result of training.

The cut of continuum suppression output: $ContProb < 0.4$.

To do fit process, transform this variable: $ContProb_{TRAN} = log(\frac{ContProb-0}{0.4-ContProb})$.

## Flavor Tag

We also used the method indroduced in [basf2 sofeware website](https://software.belle2.org/development/sphinx/online_book/basf2/flavor_tagging.html)

We add a cut on the output of flavor tag: $q \cdot r_{FBDT} < 0.89$


## Study of background from generic BBbar

Following the note from belle, we will remove the background from D meson decay, and do the fit.

### charm veto



But in the B2Kpipi0 analysis of BARBAR experiment, they do not remove the background from D, and considered this source of background in amplitude fit. 

## Cut Flow


cut | sigMC efficiency/% | SCF/% | Background number ($1~ab^{-1}$)
---------|----------|---------|---------
 fundemental | 35.70 | 8.76 | $3.93\times10^6$
 continuum suppression | 30.97 | 6.72 | $2.93\times10^5$
 charm veto | 29.93 | 6.84 | $2.81\times10^5$
 flavor tag | 29.72 | 6.85 | $2.76\times10^5$

## Fit to MC sample

### correlation between $\Delta E$ and $ContProb_{TRAN}$

Component | Correlation factor
---------|---------
Truth-matched signal      | -0.51%
SCF signal                | -0.10%  
Continuum $q\bar{q}$      | 0.99%
Generic B sample        | -1.66%
peaking from B sample   | 0.88%

### fit model of different sample

Component | $\Delta E$ | $ContProb_{TRAN}$
---------|---------|--------|
Truth-matched signal      | -0.51% |
SCF signal                | -0.10% | 
Continuum $q\bar{q}$      | 0.99% |
Generic B sample        | -1.66% |
peaking from B sample   | 0.88% |