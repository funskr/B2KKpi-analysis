
# Study of decay $B^0(\bar{B^{0}})\to K^+K^-\pi^0$

😀😄😄

basf2 version: `release-06-01-12`

## TO DO

- [ ] use git to manager my analysis files
- [ ] add flavor tag
- [ ] Amplitude analysis

## Update logs

Time | Description |
------- | ------- |
[2023](./notes/log/2023.md) |  The first month to log

<details><summary>
 conda channel (experimental)
</summary><p>

A pre-built conda package (Linux only) is also provided, just run following
command to install it.
```
conda config --add channels jiangyi15
conda install tf-pwa
```
</p></details>

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


## Flavor Tag

We also used the method indroduced in [basf2 sofeware website](https://software.belle2.org/development/sphinx/online_book/basf2/flavor_tagging.html)


## Study of background from generic BBbar

Following the note from belle, we will remove the background from D meson decay, and do the fit.

But in the B2Kpipi0 analysis of BARBAR experiment, they do not remove the background from D, and considered this source of background in amplitude fit. 
